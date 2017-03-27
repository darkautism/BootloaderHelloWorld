#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#define CONSOLE_COL_MAX 80
#define CONSOLE_ROW_MAX 25
#define PROTSEG "0x68000"
#define CODE32_SEG "0x08"
#define DATA32_SEG "0x10"
#define CODE16_SEG "0x18"
#define DATA16_SEG "0x20"
asm(".code32");
asm (
	"movw   $"DATA32_SEG", %ax\n"
	"movw   %ax, %ds\n"
	"movw   %ax, %es\n"
	"movw   %ax, %fs\n"
	"movw   %ax, %gs\n"
	"movw   %ax, %ss\n"
	"movl   $"PROTSEG", %eax\n"
	"movl   %eax, %esp\n"
	"movl   %eax, %ebp\n"
	"jmp	main"
);

uint8_t * console_buffer = (uint8_t*) 0xB8000;
int console_cursor = 0;

void console_init() {
	console_buffer = (uint8_t*) 0xB8000;
	console_cursor=0;
	for ( int i = 0 ; i < CONSOLE_COL_MAX * CONSOLE_ROW_MAX; i++ ) {
		console_buffer[i*2] = ' ';
		console_buffer[i*2+1] = 0x07;
	}
}

void console_putchar(char c) {
 	switch (c) {
		case '\r' :
			console_cursor -= console_cursor%CONSOLE_COL_MAX;
			break;
		case '\n' :
			console_cursor += 80-console_cursor%CONSOLE_COL_MAX;
			break;
		default:
			console_buffer[console_cursor*2] = c;
			console_buffer[console_cursor*2+1] = 0x07;
			console_cursor++;
			break;
	}
}

void console_putstr(char * c) {
	while (*c)
		console_putchar(*c);
}

int __itoa ( int value, char * str, int base ) {
	int pos=0;
	if (value) {
		pos=__itoa ( value/base, str, base );
		str[pos]=value%base;
		if(str[pos]>'9')
			str[pos]+=8;
		return 1+pos;
	}
	return 0;
}

char *  itoa ( int value, char * str, int base ) {
	__itoa(value/base, str, base);
	return str;
}

static int console_printf( const char *format, va_list args ) {
	int pc=0;
	char buf_int[13];
	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			if( *format == 'x' ) {
				pc+=__itoa ( va_arg( args, int ), buf_int, 16 );
			}
		} else {
			pc++;
			console_putchar(*format);
		}
	}
	va_end(args);
	return pc;
}

int printf(const char *format, ...) {
	va_list args;
	va_start( args, format );
	return console_printf( format, args );
}

static inline uint16_t inb(uint16_t port) {
	uint16_t ret;
	asm volatile ( "inw %1, %0"
		: "=a"(ret)
		: "Nd"(port) );
	return ret;
}

char waitkey() { 
	char k;
	while ((k=inb(0x60))<128);
	// while (inb(0x60)>128);
	return k;
}


/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
void __attribute__((noreturn)) main(){
	console_init();
	printf("Hello, Kernel!\n");
	console_putchar('Y');
	while (1);
	printf("Hello, Kernel!\n");
	console_putstr("Hello, Kernel!\n");
	uint16_t k, t;
	//while (1);
	while (1) {
		k=inb(0x60);
		//printf("0x%x", k);
	}
 	while(1);
}

