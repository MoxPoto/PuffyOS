#ifndef GDT_H
#define GDT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */
struct gdt_entry
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */


/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */
extern void gdt_flush();

extern void setupGDT(void);


#endif 