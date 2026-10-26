@ The interrupt handler the BIOS calls (REG_IRQ_VECTOR), in IRQ mode, having
@ saved r0-r3, r12 and lr. IrqHandler (irq.iwram.c) handles the interrupts;
@ when it returns nonzero, the sound's work for a frame runs too
@ (Audio_Background), in System mode, on the game's stack, with interrupts
@ on: it takes about a fifth of a frame, and the display's interrupts can't
@ wait.

    .syntax unified
    .arm
    .section .iwram.text.irq_entry, "ax", %progbits
    .p2align 2

    .global IrqEntry
    .type IrqEntry, %function
IrqEntry:
    push    {lr}
    bl      IrqHandler
    cmp     r0, #0
    beq     1f
    mrs     r0, spsr            @ (a nested interrupt changes it)
    push    {r0}
    msr     cpsr_c, #0x1f       @ System mode, interrupts on
    push    {lr}                @ the interrupted code's
    ldr     r0, =Audio_Background
    mov     lr, pc
    bx      r0
    pop     {lr}
    msr     cpsr_c, #0x92       @ IRQ mode, interrupts off
    ldr     r0, =g_audio_background_running
    mov     r1, #0
    strb    r1, [r0]
    pop     {r0}
    msr     spsr_cxsf, r0
1:
    pop     {lr}
    bx      lr
    .size IrqEntry, . - IrqEntry
    .ltorg
