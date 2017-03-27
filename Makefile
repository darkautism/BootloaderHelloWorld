
defaults: kernel bootstrip
	@cat boot.bin main.bin > bootloader.bin

kernel: main.c main.ld
	@gcc -std=gnu99 ${CFLAGS} -c -O2 -ffreestanding -m32 -Wall -I. -o main.o main.c
	@ld -melf_i386 -static -Tmain.ld -nostdlib --nmagic -o main.elf main.o
	@objcopy -O binary main.elf main.bin
	

stage2_size: kernel
	@set dummy `ls -l main.bin`; echo "#define STAGE2_SIZE $$6" > stage2_size.h

bootstrip: boot.c boot.ld stage2_size
	@gcc -std=gnu99 ${CFLAGS} -c -Os -ffreestanding -m32 -Wall -I. -o boot.o boot.c
	@ld -melf_i386 -static -Tboot.ld -nostdlib --nmagic -o boot.elf boot.o
	@objcopy -O binary boot.elf boot.bin
	
clean:
	rm -rf boot.elf boot.bin boot.o \
	main.o main.bin main.elf \
	stage2_size.h \
	bootloader.bin
