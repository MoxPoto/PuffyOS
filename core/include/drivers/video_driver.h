#include <vendor/multiboot.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef VID_DRIVER_H 
#define VID_DRIVER_H

extern multiboot_info_t* videoInfo;
extern void video_initialize(multiboot_info_t* grubInfo);
extern void video_displayCrash(void);

struct FontSpaceInfo {
    int width;
    int height;
};

struct Color3 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// primitves will be:
// rectangles..
// circles..
// and most interesting for output:
// characters!!

extern struct FontSpaceInfo video_drawchar(uint32_t x, uint32_t y, struct Color3 col, char target);

#endif