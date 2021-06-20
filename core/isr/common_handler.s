# Does literally nothing 

.section .text
.global common_asm_wrap
.type common_asm_wrap, @function 

common_asm_wrap:
    pushal
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    popal
    iret
