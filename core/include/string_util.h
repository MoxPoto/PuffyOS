#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* A utility function to reverse a string  */
extern void reverse(char str[], int length);
  
// Implementation of itoa()
extern char* itoa(int num, char* str, int base);

extern size_t strlen(const char* str);


#endif