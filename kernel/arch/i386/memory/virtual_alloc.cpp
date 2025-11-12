#include <kernel/memory.h>

void* valloc(uint64_t pages);
void vfree(void* addr, uint64_t pages);
