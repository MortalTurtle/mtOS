#pragma once
extern "C" void* get_physaddr(void* virtualaddr);
extern "C" void map_page(void* physaddr, void* virtualaddr, unsigned int flags);
