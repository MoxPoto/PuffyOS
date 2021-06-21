#ifndef TIME_H
#define TIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void(*TimeEventFunction)();

extern void c_time_handle(void); // ISR for the RTC chip, which goes off approximately at 1024 hz
extern void initialize_time(void);
extern float get_kernel_time(void);
extern void connect_time_event(TimeEventFunction func);

#endif 