#include <ports.h>
#include <kernel.h>
void NMI_enable(void) {
    outportb(0x70, inportb(0x70) & 0x7F);
    inportb(0x71);
}
 
void NMI_disable(void) {
    outportb(0x70, inportb(0x70) | 0x80);
    inportb(0x71);
}

void enable_interrupts(void) {
    NMI_enable();
    asm volatile("sti");
}

void disable_interrupts(void) {
    NMI_disable();
    asm volatile("cli");
}


__attribute__((noreturn))
void exception_handler(void);
void exception_handler(void) {
    kernel_crash();
    
}