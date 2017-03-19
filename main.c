#include <stddef.h>
#include <stdint.h>

#define CONSOLE_COL_MAX 80
#define CONSOLE_ROW_MAX 25

uint8_t * console_buffer = (uint8_t*) 0xB8000;
int console_cursor = 0;

void console_init() {
	for ( int i = 0 ; i < CONSOLE_COL_MAX * CONSOLE_ROW_MAX; i++ ) {
		console_buffer[i*2] = ' ';
		console_buffer[i*2+1] = 0x07;
	}
}

void console_putchar(char c) {
	console_buffer[console_cursor*2] = c;
	console_buffer[console_cursor*2+1] = 0x07;
	console_cursor++;
}

void console_putstr(char * c) {
	while (*c) {
		switch (*c) {
			case '\r' :
				console_cursor -= console_cursor%CONSOLE_COL_MAX;
				break;
			case '\n' :
				console_cursor += 80-console_cursor%CONSOLE_COL_MAX;
				break;
			default:
				console_putchar(*c);
		}
		c++;
	}
}


/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
void __attribute__((noreturn)) __attribute__((section("__start"))) main(){
	console_init();
	console_putstr("Hello, Kernel!\n");
 	while(1);
}

