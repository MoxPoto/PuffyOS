#include <vendor/multiboot.h>
#ifndef VID_DRIVER_H 
#define VID_DRIVER_H

extern multiboot_info_t* videoInfo;
extern void video_initialize(multiboot_info_t* grubInfo);

#endif