#!/bin/bash

# Adjustable
export OS_NAME_PO=puffyos.bin
export CD_NAME_PO=puffyos.iso

# Create the boot object (and other assembler files)

for FILE in $(find ./ -type f -iregex ".*\.o")
do
    i686-elf-as $FILE -o core/$FILE
done 

# Compile the kernel (and other files)
# note 2 self: -g is debug!!
i686-elf-gcc -I core/include -g -c $(find core/ -type f -iregex ".*\.c") -std=gnu99 -ffreestanding -O2 -Wall -Wextra

 
# finally, link
i686-elf-gcc -T core/linker.ld -o $OS_NAME_PO -ffreestanding -O2 -nostdlib core/*.o -lgcc

# Create a cdrom image of our operating system AND grub

mkdir -p isodir/boot/grub

cp $OS_NAME_PO isodir/boot/$OS_NAME_PO
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o $CD_NAME_PO isodir

# let's cleanup now

mkdir -p built_os
mv $OS_NAME_PO built_os
mv $CD_NAME_PO built_os

rm *.o 
rm core/*.o 

echo "Done building!"