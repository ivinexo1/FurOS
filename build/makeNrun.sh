i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/kernel.c -o ./objfiles/kernel.o
i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/vga_driver.c -o ./objfiles/vga_driver.o
i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/font.c -o ./objfiles/font.o
i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/string.c -o ./objfiles/string.o
i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/ports.c -o ./objfiles/ports.o
i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/idt.c -o ./objfiles/idt.o
i386-elf-gcc -ffreestanding -m32 -W -g -c ../src/isr.c -o ./objfiles/isr.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/keyboard.c -o ./objfiles/keyboard.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/terminal.c -o ./objfiles/terminal.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/acpi.c -o ./objfiles/acpi.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/PCI.c -o ./objfiles/pci.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/paging.c -o ./objfiles/paging.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/shell.c -o ./objfiles/shell.o
i386-elf-gcc -ffreestanding -m32 -g -c ../src/mouseDriver.c -o ./objfiles/mouseDriver.o
nasm ../src/isrhandler.asm -f elf -o ./objfiles/isrhandler.o
nasm ../src/enablePaging.asm -f elf -o ./objfiles/enablePaging.o
nasm ../src/kernel_entry.asm -f elf -o ./objfiles/kernel_entry.o
i386-elf-ld -o ./binfiles/full_kernel.bin -Ttext 0x20000 ./objfiles/kernel_entry.o ./objfiles/kernel.o ./objfiles/vga_driver.o ./objfiles/string.o ./objfiles/ports.o ./objfiles/idt.o ./objfiles/isr.o ./objfiles/isrhandler.o ./objfiles/acpi.o ./objfiles/pci.o ./objfiles/paging.o ./objfiles/enablePaging.o ./objfiles/terminal.o ./objfiles/keyboard.o ./objfiles/shell.o ./objfiles/mouseDriver.o ./objfiles/font.o --oformat binary
nasm ../src/boot.asm -f bin -o ./binfiles/boot.bin
cat ./binfiles/boot.bin ./binfiles/full_kernel.bin >./binfiles/everything.bin
cat ./binfiles/everything.bin zeroes.bin >OS.bin
qemu-img create drive.img 500M
qemu-system-x86_64 -machine pc -drive format=raw,file="OS.bin",index=0,if=floppy \
  -drive id=disk,file=drive.img,if=none \
  -device ahci,id=ahci,bus=pci.0 \
  -device ide-hd,drive=disk,bus=ahci.0 \
  -m 128M
#qemu-system-i386 OS.bin
