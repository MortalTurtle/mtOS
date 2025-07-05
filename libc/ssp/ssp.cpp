#include <ssp.h>
#include <stdint.h>
#include <stdlib.h>

extern "C" uintptr_t __stack_chk_guard;

extern "C"
__attribute__((noreturn))
void __stack_chk_fail(void) {
  #if __STDC_HOSTED__
    abort();
  #endif
  #if defined(__is_kernel)
    panic("Stack smashing detected");
  #endif
}