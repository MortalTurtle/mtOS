#pragma once
#include <stdint.h>

void init_virtual_allocator();

void* valloc(uint64_t pages);
void vfree(void* addr, uint64_t pages);

void* kvalloc(uint64_t pages);
void kvfree(void* addr, uint64_t pages);
