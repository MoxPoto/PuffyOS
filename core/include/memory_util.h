#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel.h>
#include <vendor/multiboot.h>

#define MEMORY_PADDING 16384
// 16 kb


extern uint32_t kernel_find_address(void);
extern uint32_t kernel_get_memory(void);
extern void* malloc(size_t size);
extern void* calloc(size_t size);
extern void free(size_t size);
extern void initialize_memory(multiboot_info_t* grubInfo);
#endif 