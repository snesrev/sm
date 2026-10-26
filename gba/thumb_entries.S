@ Thumb entries in the cartridge for ARM functions in IWRAM that the game's
@ Thumb code calls often. The linker's own veneer for such a call is a bx pc
@ and an ldr pc, each refilling the pipeline from the cartridge: 27 cycles. An
@ entry loads the address and jumps: about 12. It uses r3, so it's for
@ functions with up to three arguments. The function returns to the caller.

    .syntax unified
    .thumb

.macro thumb_entry name, arm
    .section .romtext.\name, "ax", %progbits
    .p2align 2
    .global \name
    .type \name, %function
    .thumb_func
\name:
    ldr     r3, 1f
    bx      r3
1:  .word   \arm
.endm

    thumb_entry WriteReg, WriteRegArm
    thumb_entry WriteRegWord, WriteRegWordArm
