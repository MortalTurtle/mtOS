#include "kernel/memory_manager.h"

#define VIRT_KERNEL_START 0xC0000000
#define VIRT_KERNEL_END   0xFFFFFFFF

void* kmalloc(uint64_t size) {}
void* kfree(uint64_t size) {}
