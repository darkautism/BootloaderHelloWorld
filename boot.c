
#include <stage2_size.h>
asm(".code16gcc\n");

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __NORETURN  __attribute__((noreturn))

struct lba_reader_packet
{
	unsigned short struct_size;				/* struct's size ( 0x10 or  0x18 )*/
	unsigned short sectors;					/* read size of sectors */
	unsigned int bootloader_load_address;	/* buffer address */
	unsigned long long start_sector;
} ;

struct gdtdesc_t  {
	unsigned short limit;
	unsigned long gdtaddr;
} __attribute__ ((packed));

struct gdt_t {
	unsigned short limit;
	unsigned short base;
	unsigned char type[4];
} __attribute__ ((packed));

static struct gdt_t gdtarr[5] = {
	{},
	{.limit=0xFFFF, .type={0, 0x9A, 0xCF, 0}},
	{.limit=0xFFFF, .type={0, 0x92, 0xCF, 0}},
	{.limit=0xFFFF, .type={0, 0x9E, 0, 0}},
	{.limit=0xFFFF, .type={0, 0x92, 0, 0}}
};

static struct gdtdesc_t gdtdesc = {
	.limit = 0x27,
	.gdtaddr = (unsigned long) &gdtarr
};

static unsigned short sz_stage2 = ((STAGE2_SIZE+511)>>9);

// static unsigned short sz_stage2;

void __NOINLINE __REGPARM print(const char *s) {
	while(*s) {
		asm volatile (
			"movw	$0x0001, %%bx\n"
			"movb	$0x0E, %%ah\n"
			"movb	%b0, %%al\n"
			"int	$0x10"
			: 
			: "al"(*s++):"ah","bx");
	}
}

void __NOINLINE __REGPARM lba_load_sector(unsigned char boot_drive, struct lba_reader_packet * lbapkt) {
	short drive=boot_drive;
	while ( sz_stage2--) {
		lbapkt->bootloader_load_address+=1<<21;
		lbapkt->start_sector++;
		asm volatile goto(
			"int	$0x13\n"
			"jc	%l[failed]"
			: 
			: "a"(0x4200), "d"(drive), "S"(lbapkt)
			:"cc"
			: failed);
	}
	asm volatile("Loop2: \n jmp Loop2");
	return;
failed:
	print("Boot failed");
	asm volatile("Loop: \n jmp Loop");
}

/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
void __NORETURN __attribute__((section("__start"))) main(){

 	unsigned char bios_drive = 0xff;
 	struct lba_reader_packet *lbapkt=0;
	asm volatile(
		"movb %%dl, %b0\n"
		"mov  %%esi, %1\n"
		: "=b"(bios_drive), "=m"(lbapkt) :: "dl","si"); /* MBR will pass packet address by si */
	//print("Hello, Bootloader!\r\n");
	lba_load_sector(bios_drive, lbapkt);
	/* goto protected mode */
	asm volatile(
		"cli\n"
		"ADDR32	lgdtl	%0\n"
		"movl	%%cr0, %%eax\n"
		"orl	$1, %%eax\n"
		"movl	%%eax, %%cr0\n"
		"DATA32	ljmp	$0x08,$0x8200"
		:
		: "m"(gdtdesc):"eax", "edx");
	goto *((void*) 0x8200 );
}
