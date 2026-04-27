# vprobe
> A low level profiler for Linux served as `vprobe.h`. Cold-cache clean room + raw PMC hardware counters.

**Status: Early Development — Discovery Module Only**

---

## What's Implemented

### System Discovery (`vprobe_discover`)
Auto-discovers CPU cache topology and core count by reading directly from the Linux sysfs filesystem. No external dependencies, no shell commands — pure kernel interfaces.

Discovers and stores:
- L1 instruction cache size (bytes)
- L1 data cache size (bytes)
- L2 unified cache size (bytes)
- L3 unified cache size (bytes)
- Cache line size (bytes)
- Total online core count
- Target core index

### sysfs Size Parser (`vprobe_parse_sysfs_size`)
Parses kernel size strings with K/M/G suffix support into raw bytes. Handles both uppercase and lowercase suffixes.

### Error Handling (`vprobe_strerror`)
Structured error code system modelled after POSIX `strerror`. Every failure path returns a named error code — no silent failures.

---

## Building

```bash
git clone https://github.com/yourusername/vprobe
cd vprobe
make
./vprobe
```

Requires Linux. No external dependencies. Single header — drop `vprobe.h` anywhere.

---

## Error Codes

| Code | Value | Meaning |
|------|-------|---------|
| `VPROBE_SUCCESS` | 0 | All good |
| `VPROBE_ERR_PROCS_ONLN` | -1 | Could not determine online core count |
| `VPROBE_ERR_INVALID_CORE` | -2 | Target core index out of bounds |
| `VPROBE_ERR_SYSFS_READ` | -3 | Failed to open sysfs file |
| `VPROBE_ERR_CACHE_SIZE_READ` | -4 | Failed to read cache size |
| `VPROBE_ERR_CACHE_LINE_SIZE_READ` | -5 | Failed to read cache line size |

---

## Data Sources

| Field | Source |
|-------|--------|
| Cache sizes | `/sys/devices/system/cpu/cpu{n}/cache/index{n}/size` |
| Cache line size | `/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size` |
| Core count | `sysconf(_SC_NPROCESSORS_ONLN)` |

---

## Roadmap

- [ ] Dynamic cache type detection via sysfs `type` file (AMD/Intel/ARM portability)
- [ ] RAM size discovery via `/proc/meminfo`
- [ ] Cache flush logic (32MB+ volatile sweep)
- [ ] `perf_event_open` PMC integration
- [ ] `VPROBE_START` / `VPROBE_STOP` macros with barrier ordering
- [ ] Null probe calibration
- [ ] Cycle / instruction / cache miss reporting
- [ ] Comparison table with IPC and MPKI

---

## License

MIT — Copyright (c) 2026