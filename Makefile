all:
	echo "Compiling..."
	i686-os365-as boot.s -o boot.o > boot.log
	i686-os365-as paging.s -o paging.o
	yasm ring3.s -o ring3.o -f elf32
	i686-os365-g++ -c kernel.cpp -o kernel.o -O2 -ffreestanding -Wall -Wextra -Wmaybe-uninitialized -fno-exceptions -fno-rtti -fpermissive> kernel.log
	echo "done."
	i686-os365-ld -T linker.ld -o os365.bin -O2 -nostdlib boot.o kernel.o paging.o ring3.o sysroot/usr/lib/libc.a sysroot/usr/lib/crt0.o > linker.log
	echo "Creating ISO..."
	cp os365.bin isodir/boot/os365.bin
	grub-mkrescue -o os365.iso isodir > isomake.log
	echo "done. Running in QEMU..."
	qemu-system-i386 -cdrom os365.iso
clean:
	rm *.o *.iso *.bin
qemu:
	qemu-system-i386 -cdrom os365.iso
