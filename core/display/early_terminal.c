// this damned file deleted it self once
// i cant tell if I deleted..
// i checked my bash history 3 times..

#include <early_terminal.h>
#include <string_util.h>

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

struct TerminalState TERMINAL_STATE;
 
void terminal_initialize(void) 
{
	TERMINAL_STATE.terminal_row = 0;
	TERMINAL_STATE.terminal_column = 0;
	TERMINAL_STATE.terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

}
 
void terminal_setcolor(uint8_t color) 
{
	TERMINAL_STATE.terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	TERMINAL_STATE.terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) 
{
    if (c == '\n') {
        // go down a row
        TERMINAL_STATE.terminal_row++;
        TERMINAL_STATE.terminal_column = 0;

        return;
    } else {
	    terminal_putentryat(c, TERMINAL_STATE.terminal_color, TERMINAL_STATE.terminal_column, TERMINAL_STATE.terminal_row);
	}
    
    if (++TERMINAL_STATE.terminal_column == VGA_WIDTH) {
		TERMINAL_STATE.terminal_column = 0;
		if (++TERMINAL_STATE.terminal_row == VGA_HEIGHT)
			TERMINAL_STATE.terminal_row = 0;
	}
}
 
void terminal_write(const char* data, size_t size) 
{    
    // adds a [+] suffix

	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
    uint8_t original = TERMINAL_STATE.terminal_color;

    terminal_putchar('[');
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    terminal_putchar('+');
    terminal_setcolor(original);
    terminal_putchar(']');
    terminal_putchar(' ');

	terminal_write(data, strlen(data));
}

void terminal_error(const char* data) {
    uint8_t original = TERMINAL_STATE.terminal_color;
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_WHITE));

	terminal_write(data, strlen(data));

    terminal_setcolor(original);
}