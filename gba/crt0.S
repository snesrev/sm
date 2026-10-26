@ GBA startup code. The cartridge entry point at 0x08000000 is a single ARM
@ branch over the 192-byte ROM header (see header.c) to _reset, which sets
@ up the stacks, initializes IWRAM and EWRAM, and runs gba_main().
@ https://problemkaputt.de/gbatek.htm#gbacartridgeheader

    .syntax unified
    .arm

    .section .crt0.entry, "ax", %progbits
    .global _start
_start:
    b       _reset

    .section .crt0.text, "ax", %progbits
_reset:
    @ The BIOS sets these too, but don't depend on how we were booted.
    @ Stacks sit at the top of IWRAM, below the BIOS's reserved area.
    mov     r0, #0x12           @ IRQ mode
    msr     cpsr_c, r0
    ldr     sp, =__sp_irq
    mov     r0, #0x1F           @ System mode, where gba_main() runs
    msr     cpsr_c, r0
    ldr     sp, =__sp_sys

    @ Copy initialized data from ROM to IWRAM (.iwram) and EWRAM (.data).
    ldr     r0, =__iwram_lma
    ldr     r1, =__iwram_start
    ldr     r2, =__iwram_end
1:  cmp     r1, r2
    ldrlo   r3, [r0], #4
    strlo   r3, [r1], #4
    blo     1b
    ldr     r0, =__data_lma
    ldr     r1, =__data_start
    ldr     r2, =__data_end
2:  cmp     r1, r2
    ldrlo   r3, [r0], #4
    strlo   r3, [r1], #4
    blo     2b

    @ Zero .iwram_bss in IWRAM and .bss in EWRAM.
    mov     r3, #0
    ldr     r1, =__iwram_bss_start
    ldr     r2, =__iwram_bss_end
3:  cmp     r1, r2
    strlo   r3, [r1], #4
    blo     3b
    ldr     r1, =__bss_start
    ldr     r2, =__bss_end
4:  cmp     r1, r2
    strlo   r3, [r1], #4
    blo     4b

    @ ARMv4T has no BLX: call through MOV LR, PC / BX, which also
    @ interworks with Thumb code.
    ldr     r0, =gba_main
    mov     lr, pc
    bx      r0
5:  b       5b                  @ gba_main() never returns; park if it does

    .pool
