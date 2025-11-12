#include <kernel/multiboot.h>

static const multiboot_info* mb_info = nullptr;

void multiboot_parse(uint32_t multiboot_info_phys) {
  mb_info = arch_multiboot_parse(multiboot_info_phys);
}

const multiboot_info* multiboot_get_info() { return mb_info; }
