/* Create multiboot header constants */
.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set GIMME_VIDEO, 1<<2
.set VIDEOMODE_TYPE, 0 # 0 for linear graphics data
.set VIDEO_WIDTH, 640
.set VIDEO_HEIGHT, 480
.set BPP_VIDEO, 32

.set FLAGS, ALIGN | MEMINFO | GIMME_VIDEO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* Create the damn header */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0 # Useless shit
.long 0
.long 0
.long 0
.long 0
.long VIDEOMODE_TYPE
.long VIDEO_WIDTH
.long VIDEO_HEIGHT
.long BPP_VIDEO

# IDT
.macro isr_err_stub id 
    isr_stub_\id: 
        mov $\id, %EAX
        call exception_handler
        iret 
.endm

.macro isr_no_err_stub id 
    isr_stub_\id: 
        mov $\id, %EAX
        call exception_handler
        iret 
.endm

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

.global isr_stub_table
isr_stub_table: # Help me
    .long isr_stub_0
    .long isr_stub_1
    .long isr_stub_2
    .long isr_stub_3
    .long isr_stub_4
    .long isr_stub_5
    .long isr_stub_6
    .long isr_stub_7
    .long isr_stub_8
    .long isr_stub_9
    .long isr_stub_10
    .long isr_stub_11
    .long isr_stub_12
    .long isr_stub_13
    .long isr_stub_14
    .long isr_stub_15
    .long isr_stub_16
    .long isr_stub_17
    .long isr_stub_18
    .long isr_stub_19
    .long isr_stub_20
    .long isr_stub_21
    .long isr_stub_22
    .long isr_stub_23
    .long isr_stub_24
    .long isr_stub_25
    .long isr_stub_26
    .long isr_stub_27
    .long isr_stub_28
    .long isr_stub_29
    .long isr_stub_30
    .long isr_stub_31

/* Setup the stack */

.global gdt_flush     # Allows the C code to link to this
.extern gp            # Says that '_gp' is in another file
gdt_flush:
    lgdt [gp]        # Load the GDT with our '_gp' which is a special pointer

    jmp $0x08,$flush2   # 0x08 is the offset to our code segment: Far jump!
flush2:
    mov $0x10, %AX      # 0x10 is the offset in the GDT to our data segment
    mov %AX, %DS 
    mov %AX, %ES 
    mov %AX, %FS 
    mov %AX, %GS 
    mov %AX, %SS

    ret             # Returns back to the C code!


.section .bss
.align 16
stack_bottom:
    .skip 16384 # 16 kib
stack_top:
.section .text
.global _crash
.type _crash, @function 
_crash:
    # shutdown the system
    mov $0x01, %EAX; // Stop code
    call kernel_crash;

2:  hlt;
    jmp 2;

.section .text
.global _start
.type _start, @function 
_start:


    mov $stack_top, %esp


    /* Start kernel */

    
    call kernel_main;

    cli
1:  hlt # Halt CPU
    jmp 1

.size _start, . - _start
