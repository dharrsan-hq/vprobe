#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VPROBE_SUCCESS 0
#define VPROBE_ERR_PROCS_ONLN -1
#define VPROBE_ERR_INVALID_CORE -2
#define VPROBE_ERR_SYSFS_READ -3
#define VPROBE_ERR_CACHE_SIZE_READ -4
#define VPROBE_ERR_CACHE_LINE_SIZE_READ -5

typedef struct {
    uint64_t l1i_size_bytes;
    uint64_t l1d_size_bytes;
    uint64_t l2_size_bytes;
    uint64_t l3_size_bytes;
    uint64_t cache_line_size_bytes;
    uint8_t num_cores;
    uint8_t target_core; 
} vprobe_core_t;

static inline const char* vprobe_strerror(int errnum){
    switch (errnum){
        case VPROBE_ERR_INVALID_CORE: return "Target core out of bounds";
        case VPROBE_ERR_SYSFS_READ: return "Failed to read filesystem";
        case VPROBE_ERR_CACHE_SIZE_READ: return "Failed to read cache sizes";
        case VPROBE_ERR_CACHE_LINE_SIZE_READ: return "Failed to read cache line size";
        
        default: return 0;
    }
}

static inline uint64_t vprobe_parse_sysfs_size(const char* buf){
    char* endptr;
    uint64_t value = strtoll(buf, &endptr, 10);

    while (*endptr && isspace(*endptr)) endptr++;

    switch (*endptr) {
        case 'K': case 'k': return value * 1024;
        case 'M': case 'm': return value * 1024* 1024;
        case 'G': case 'g': return value * 1024 * 1024 * 1024;
        
        default:            return value; // just bytes
    }
}

static inline int vprobe_discover(vprobe_core_t* core, uint8_t target) {
    // find total number of cores; setting num_cores
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 1) 
        return VPROBE_ERR_PROCS_ONLN;
    core->num_cores = (uint8_t)nprocs;

    // setting target_core
    if (target<nprocs)
        core->target_core = target;
    else
     return VPROBE_ERR_INVALID_CORE;


    // finding all cache sizes: l1i, l1d, l2, l3 based on target_core
    char path[256];
    char buf[32];
    for (uint8_t i=0; i<=3; i++){
        snprintf(path, sizeof(path), "/sys/devices/system/cpu/cpu%u/cache/index%u/size", target, i);

        FILE* f = fopen(path, "r");
        if (!f) return VPROBE_ERR_SYSFS_READ;

        if (fgets(buf, sizeof(buf), f)){
            uint64_t bytes = vprobe_parse_sysfs_size(buf);

            switch (i) {
                case 0: 
                    core->l1d_size_bytes = bytes;
                    break;
                case 1:
                    core->l1i_size_bytes = bytes;
                    break;
                case 2:
                    core->l2_size_bytes = bytes;
                    break;
                case 3:
                    core->l3_size_bytes = bytes;
                    break;
            }
        }
        fclose(f);
    }

    // finding cache line size
    snprintf(path,sizeof(path), "/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size");
    FILE* f = fopen(path, "r");
    if (!f) return VPROBE_ERR_SYSFS_READ;

    if (fgets(buf, sizeof(buf), f)){
        core->cache_line_size_bytes = (uint64_t)strtoull(buf, NULL, 10);
    }
    fclose(f);

    return VPROBE_SUCCESS;
}

