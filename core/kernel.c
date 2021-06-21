#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <early_terminal.h>
#include <string_util.h>
#include <cpuid.h>
#include <kernel.h>
#include <interrupts.h>
#include <pic.h>
#include <gdt.h>
#include <vendor/multiboot.h>

// Create our IDT
__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void idt_init(void);
void idt_init(void) {
    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
 
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        // idt[vector] = true;
    }
 
    asm volatile ("lidt %0" : : "memory"(idtr)); // load the new IDT
    asm volatile ("sti"); // set the interrupt flag
}

void detectVendor(char* vendor) {
	uint32_t empty = 0;

	asm("movl $0, %%eax"
		:
		:
		:"%eax");

	asm("cpuid"); // Call cpuid

	uint32_t res1 = 0;
	uint32_t res2 = 0;
	uint32_t res3 = 0;

	asm("movl %%ebx, %0"
		:"=r"(res1)
		:
		:"%ebx");

		asm("movl %%edx, %0"
		:"=r"(res2)
		:
		:"%edx");

		asm("movl %%ecx, %0"
		:"=r"(res3)
		:
		:"%ecx");

	for (int i = 0; i < 4; i++) {
		vendor[i] = (res1 >> (i * 8) & 0xFF);
	}
	
	for (int i = 0; i < 4; i++) {
		vendor[i + 4] = (res2 >> (i * 8) & 0xFF);
	}

	for (int i = 0; i < 4; i++) {
		vendor[i + 8] = (res3 >> (i * 8) & 0xFF);
	}

	vendor[12] = '\n';
	vendor[13] = '\0';

}

// as the name suggests, look for a void* keyboard assembly wrapper
extern void keyboard_asm_wrap(void);
extern void common_asm_wrap(void);

void c_key_handle(void) {
	terminal_writestring("Detected a key press!!\n");
	unsigned char scan_code = inportb(0x60);
	// fun fact: the keyboard doesnt continue sending any more interrupts until you read the scan
	// code
	
	PIC_sendEOI(1);
	
	return;
}

void kernel_crash(void) {
	int stopCode = 1337;
	asm("mov %%eax, %0" : "=r"(stopCode) : :"%eax");


	char stopCodeStr[50];
	itoa(stopCode, stopCodeStr, 10);
	terminal_error("\nKERNEL PANIC!\n");
	terminal_error("STOP CODE IS: ");
	terminal_error(stopCodeStr);


	for (;;) {
		asm("hlt");
	}
}

void kernel_main(void)
{
	// thank u grub.. for giving me this info

	multiboot_info_t* grubInfo;
	asm("mov %%ebx, %0" : "=r"(grubInfo) : :"%ebx");
	// The address of the pointer to the bootloader info is stored in the EBX register
	
	disable_interrupts();
	setupGDT();

	terminal_initialize();
	
	terminal_writestring("PuffyOS booted..\n");
	terminal_writestring("Disabling interrupts..\n");
	
	terminal_writestring("Detecting CPU..\n");

	char vendor[14];

	detectVendor(vendor);

	terminal_writestring(vendor);

	

	

	terminal_writestring("Creating IDT..\n");

	terminal_writestring("Setting up PIC..\n");
	
		
	initialize_pic();

	terminal_writestring("Installing key handler and bitmasking IRQ lines..\n");
	
	idt_set_descriptor(33, keyboard_asm_wrap, 0x8E);
	idt_set_descriptor(32, common_asm_wrap, 0x8E);

	for (int i = 0; i < 16; i++) {
		IRQ_set_mask(i);
	}

	IRQ_clear_mask(1);
	
	idt_init();
	terminal_writestring("Installed IDT!\n");
	

	enable_interrupts();

	terminal_writestring("Enabled interrupts!\n");

	uint16_t pixelwidth = grubInfo->framebuffer_bpp / 8;
	
	for(unsigned int x = 0; x < grubInfo->framebuffer_width; x++) {
		for(unsigned int y = 0; y < grubInfo->framebuffer_height; y++) {
			uint32_t index = (y * grubInfo->framebuffer_pitch + x * pixelwidth);

			unsigned char* pixel = (unsigned char*)grubInfo->framebuffer_addr;
			pixel[index] = 255;
			pixel[index + 1] = 255;
			pixel[index + 2] = 255;
		}
	}
	for(;;) {
		asm("hlt");
	}
}