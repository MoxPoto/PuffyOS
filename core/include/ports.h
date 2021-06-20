#ifndef PORT_H
#define PORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern void outportb(uint16_t port, uint8_t dataToSend);
extern uint16_t inportb(uint16_t port);
extern void io_wait(void);

#endif