#include <ports.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// These are all mainly from osdev references

void outportb(uint16_t port, uint8_t dataToSend) {
    asm volatile ( "outb %0, %1" : : "a"(dataToSend), "Nd"(port) :);
}

uint16_t inportb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void io_wait(void)
{
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}