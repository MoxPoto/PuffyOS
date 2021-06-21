#include <memory_util.h>
#include <kernel.h>
#include <string_util.h>

static uint32_t base_memory_address = 0x0;
static uint32_t max_memory_size = 0x0;
static uint32_t current_memory_address = 0x0;

// these will all help keep track of memory

static bool enoughMemoryFor(size_t size) {
    return (current_memory_address + (uint32_t)size) < max_memory_size;
}

uint32_t kernel_find_address(void) {
    return current_memory_address;
}

uint32_t kernel_get_memory(void) {
    return max_memory_size - current_memory_address;
}

void* malloc(size_t size) {
    if (!enoughMemoryFor(size)) {
        asm("mov $85, %%eax" :::"%eax");
        kernel_crash();
    }

    uint32_t newMemory = current_memory_address;
    current_memory_address += size;

    return (void*)newMemory;
}

void* calloc(size_t size) {
    void* newMem = malloc(size);

    for (size_t i = 0; i < size; i++) {
        *((int*)newMem + i) = 0;
    }

    return newMem;
}

void free(size_t size) {
    current_memory_address -= size;
}

void initialize_memory(multiboot_info_t* grubInfo) {
    if(!(grubInfo->flags >> 6 & 0x1)) {
		asm("mov $135, %%eax" ::: "%eax");
        kernel_crash();
    } else {
		terminal_writestring("Got valid memory map information.. mapping\n");
		
		for(int i = 0; i < grubInfo->mmap_length; 
			i += sizeof(multiboot_memory_map_t)) 
		{
			multiboot_memory_map_t* mmmt = 
				(multiboot_memory_map_t*) (grubInfo->mmap_addr + i);
	
			if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE && mmmt->len != 0) {
				terminal_writestring("Found usable memory block!\n");

                if (mmmt->len > max_memory_size) {
                    max_memory_size = mmmt->len - MEMORY_PADDING;
                    base_memory_address = mmmt->addr + MEMORY_PADDING;
                }
			}
		}
	}

    current_memory_address = base_memory_address;

    char memoryLeftStr[50];
    itoa(kernel_get_memory(), memoryLeftStr, 10);

    terminal_writestring("Current memory in system:\n");
    terminal_writestring(memoryLeftStr);
    terminal_writestring("\n");   

    int* test = malloc(sizeof(int));
    (*test) = 13;

    char testStr[8];
    itoa(*test, testStr, 10);

    terminal_writestring("RAM Test result:\n");
    terminal_writestring(testStr);
    terminal_writestring("\n");   

    free(sizeof(int));
}