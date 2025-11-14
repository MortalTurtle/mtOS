#pragma once
#include <stdint.h>

void init_kernel_memory_allocator();
void* kmalloc(uint64_t size);
void kfree(uint64_t size);
