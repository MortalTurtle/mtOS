#include <ssp.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __is_kernel
#include <kernel/kernel.h>
#endif

extern "C" uintptr_t __stack_chk_guard;

extern "C"
__attribute__((noreturn))
void __stack_chk_fail(void) {
  #if __STDC_HOSTED__
    abort();
  #endif
  #ifdef __is_kernel
    panic("Stack smashing detected");
  #endif
}