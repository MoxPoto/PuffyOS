#include <time.h>
#include <ports.h>
#include <pic.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static float kernel_time = 0.f;
static const float time_delta = 0.00097f;
// 1024 hz to seconds, which is approximately the time inbetween IRQ 8 interrupts

void c_time_handle(void) {
    kernel_time += time_delta;
    PIC_sendEOI(8);

    outportb(0x70, 0x0C);	// select register C
    inportb(0x71);		// just throw away contents

    // code up there is from osdev, apparently you need to read from
    // register C in order to have the RTC chip keep propagating
    // interrupts to the kernel
}

void initialize_time(void) {
    outportb(0x70, 0x8B);		// select register B, and disable NMI
    char prev = inportb(0x71);	// read the current value of register B
    outportb(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
    outportb(0x71, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
}

float get_kernel_time(void) {
    return kernel_time;
}