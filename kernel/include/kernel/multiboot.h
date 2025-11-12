#pragma once
#include <stdint.h>

extern "C" {

#define MULTIBOOT_INFO_MEMORY 0x00000001
#define MULTIBOOT_INFO_BOOTDEV 0x00000002
#define MULTIBOOT_INFO_CMDLINE 0x00000004
#define MULTIBOOT_INFO_MODS 0x00000008
#define MULTIBOOT_INFO_MEM_MAP 0x00000040

struct multiboot_mmap_entry {
  uint32_t size;
  uint64_t addr;
  uint64_t len;
  uint32_t type;
} __attribute__((packed));

struct multiboot_module {
  uint32_t mod_start;
  uint32_t mod_end;
  uint32_t string;
  uint32_t reserved;
};

struct multiboot_info {
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  uint32_t cmdline;
  uint32_t mods_count;
  uint32_t mods_addr;
  uint32_t syms[4];
  uint32_t mmap_length;
  uint32_t mmap_addr;
  uint32_t drives_length;
  uint32_t drives_addr;
  uint32_t config_table;
  uint32_t boot_loader_name;
  uint32_t apm_table;
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
};

void multiboot_parse(uint32_t multiboot_info_phys);
const multiboot_info* arch_multiboot_parse(uint32_t multiboot_info_phys);
const multiboot_info* multiboot_get_info();
}
