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
	TERMINAL_STATE.terminal_color.r = 255;
    TERMINAL_STATE.terminal_color.g = 255;
    TERMINAL_STATE.terminal_color.b = 255;

}
 
void terminal_setcolor(struct Color3 color) 
{
	TERMINAL_STATE.terminal_color = color;
}
 
static struct FontSpaceInfo terminal_putentryat(char c, size_t x, size_t y) 
{
	struct FontSpaceInfo result = video_drawchar((uint32_t)x, (uint32_t)y, TERMINAL_STATE.terminal_color, c);
    return result;
}
 
void terminal_putchar(char c) 
{
    if (c == '\n') {
        // go down a row
        TERMINAL_STATE.terminal_row += 9;
        TERMINAL_STATE.terminal_column = 0;

        return;
    } 

	struct FontSpaceInfo spacing = terminal_putentryat(c, TERMINAL_STATE.terminal_column, TERMINAL_STATE.terminal_row);
	
    // 2 here is padding.. need to make that variable
    TERMINAL_STATE.terminal_column += spacing.width;

    if (TERMINAL_STATE.terminal_column >= videoInfo->framebuffer_width) {
		TERMINAL_STATE.terminal_column = 0;

        TERMINAL_STATE.terminal_row += spacing.height + 2;

		if (TERMINAL_STATE.terminal_row >= videoInfo->framebuffer_height)
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
    struct Color3 original = TERMINAL_STATE.terminal_color;
    struct Color3 new;

    new.r = 60;
    new.g = 255;
    new.b = 60;

    terminal_putchar('[');
    terminal_setcolor(new);
    terminal_putchar('+');
    terminal_setcolor(original);
    terminal_putchar(']');
    terminal_putchar(' ');

	terminal_write(data, strlen(data));
}

void terminal_error(const char* data) {
    struct Color3 original = TERMINAL_STATE.terminal_color;
    struct Color3 new;
    new.r = 255;
    new.g = 50;
    new.b = 50;
    terminal_setcolor(new);

	terminal_write(data, strlen(data));

    terminal_setcolor(original);
}