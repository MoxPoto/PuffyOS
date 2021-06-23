#ifndef KEY_DRIVER_H
#define KEY_DRIVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum KeyFlags {
    HeldDown   
};

struct Key {
    unsigned char ascii;
    bool pressed;
};

struct KeyInstruction {
    uint8_t scancode;
    unsigned char ascii;
    bool pressed;
};

extern struct Key* keyboard_array;
extern struct KeyInstruction* keyboard_instructions;
extern void c_key_handle(void);

extern void initialize_keyboard(void);

#endif