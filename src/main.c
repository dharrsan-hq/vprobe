#include <stdio.h>
#include <sys/select.h>
#include "vprobe.h"

int main(){
   
   vprobe_core_t core;

   int res = vprobe_discover(&core, 15);
   if (res != VPROBE_SUCCESS){
      fprintf(stderr, "VPROBE ERROR (%d): %s\n", res, vprobe_strerror(res));
      return -1;
   }

   printf("DISCOVERY:\n");
   printf("core_count: %d\n",core.num_cores);
   printf("core_target: %d\n",core.target_core);
   printf("size_cache_l1d: %lu bytes\n",core.l1d_size_bytes);
   printf("size_cache_l1i: %lu bytes\n",core.l1i_size_bytes);
   printf("size_cache_l2: %lu bytes\n",core.l2_size_bytes);
   printf("size_cache_l3: %lu bytes\n",core.l3_size_bytes);
   printf("size_cache_line: %lu bytes\n",core.cache_line_size_bytes);



   return 0; 
}