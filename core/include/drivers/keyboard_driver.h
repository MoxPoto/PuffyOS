#ifndef KEY_DRIVER_H
#define KEY_DRIVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum KeyFlags {
    HeldDown   
};

struct Key {
    char ascii;
    uint8_t flags;
};

Key keyboard_keys[128];
extern void c_key_handle(void);

#endif