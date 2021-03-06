#ifndef __VGATERM_H
#define __VGATERM_H
#include <stddef.h>
#include <stdint.h>
#include "io.h"
#if !defined(_vga)
#define _vga
#endif
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
//Thanks to OSDev.
uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
size_t strlen(const char* str)
{
	size_t ret = 0;
	while(str[ret] != 0)
	{
		ret++;
	}
	return ret;
}
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for(size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
			
		}
	}
}
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
void terminal_putchar(char c)
{
	char nums[10];
	nums[0] = '0';
	nums[1] = '1';
	nums[2] = '2';
	nums[3] = '3';
	nums[4] = '4';
	nums[5] = '5';
	nums[6] = '6';
	nums[7] = '7';
	nums[8] = '8';
	nums[9] = '9';

	if(c != '\n')
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	}
	else
	{
		terminal_column = 0 - 1;
		terminal_row++;
	}
	switch(c)
	{
	case 0: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 1: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 2: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 3: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 4: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 5: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 6: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 7: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 8: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);
	case 9: terminal_putentryat(nums[(int)c], terminal_color, terminal_column, terminal_row);

	}
	if(++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0 - 1;
		terminal_row++;
		if(++terminal_row == VGA_HEIGHT)
		{
			terminal_row = 25;
			terminal_column = 0;
			for(size_t fill; fill < 80; fill++)
			{
				//TODO: make the scrolling.
			}
		}
	}
	update_cursor(terminal_row,terminal_column);
}
void goToPos(int x, int y)
{
	terminal_column = x;
	terminal_row = y;
	update_cursor(terminal_row,terminal_column);
}
void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}
void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for(size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}
#endif
