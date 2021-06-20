# so, C functions will generate a 'ret' instruction once they're done
# this assembly wrapper generates the correct iret instruction, but also lets C
# do some of the work

.section .text
.global keyboard_asm_wrap
.type keyboard_asm_wrap, @function 

.extern c_key_handle
# extern doesnt do anything to the actual file in GAS
# but i feel like it looks better rather than relying on
# making undefined symbols automatically extern

keyboard_asm_wrap:
    pushal
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call c_key_handle
    popal
    iret