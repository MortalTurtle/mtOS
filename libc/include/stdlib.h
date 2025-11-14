#pragma once
#include "stddef.h"
extern "C" {

__attribute__((__noreturn__)) void abort(void);

void* malloc(size_t size);
void free(void* addr);
}
