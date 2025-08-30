#pragma once
#include <stdint.h>

void* kmalloc(uint64_t size);
void* kfree(uint64_t size);
