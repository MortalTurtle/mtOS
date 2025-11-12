#pragma once
#include <stdint.h>

extern "C" __attribute__((noreturn)) void panic(char* msg);

void system_setup(uint32_t multiboot_info_phys);
