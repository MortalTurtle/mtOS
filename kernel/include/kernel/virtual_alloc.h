#pragma once
#include <stdint.h>

void virtual_alloc_init();

void* valloc(uint64_t pages);
void vfree(void* addr, uint64_t pages);

void* kvalloc(uint64_t pages);
void kvfree(void* addr, uint64_t pages);
