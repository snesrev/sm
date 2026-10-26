@ Converting SNES maps to the GBA's, by hand (convert.h's Convert_MapChunks): the
@ compiler's loop spilled, with a dozen constants and pointers in play, and took
@ 46 cycles an entry pair where this takes about 30.
@
@     void Convert_MapChunks(uint32_t todo, volatile uint16_t *lo, volatile uint16_t *hi,
@                            const uint16_t *src, uint8_t *counts, uint16_t *cols,
@                            uint32_t blank, uint32_t add);
@
@ The chunks of 16 entries (8 pairs, a word each) that |todo| has, a bit each (chunk
@ c is |src| + 32 * c bytes), go to |lo| (the same offset) or, with |hi|, split in
@ two by the priority bit: where an entry has it, |hi| has the entry and |lo| |blank|,
@ and where it doesn't, the other way round. |counts|[c] and |cols|[c & 1] are kept
@ up to date with how many of chunk c's 16 entries have the priority bit.
@
@ An entry SNES-side is vhopppcc cccccccc, and GBA-side ppppvhcc cccccccc: the tile and
@ the palette bits move (the tile with |add|'s tile, in bits 0-9, added to it and
@ wrapping around, and the palette |add|'s, in bits 12-15, added), and the flips two
@ places down. The tile's carry out of bit 9 is the only one, so the palette and the
@ tile are added together and bits 10 and 11, where the flips go, cleared after.
@
@ Registers while converting a chunk:
@   r0 src (a word on at each pair)   r1 lo - src - 4          r2 hi - src - 4 (0: no hi)
@   r3 priority count (a halfword for each of a pair's entries)
@   r4 pairs to go
@   r5 0x03ff03ff  r6 0x0c000c00  r7 0x70007000  r8 0x00010001  r9 add  r10 blanks
@   r11 the entry  r12 the converted one  lr scratch
@ and between chunks r3 is |todo|, r4 0 or 2 (the bytes to cols[c & 1]) and r11
@ |counts| at the chunk. They go to [sp, #0], [sp, #4] and [sp, #8] meanwhile.

    .syntax unified
    .arm
    .section .iwram.text.convert_map, "ax", %progbits
    .p2align 2

    @ The stack: three words saved, and the 9 registers pushed, then the arguments.
    .equ A_COUNTS, 12 + 36
    .equ A_COLS, A_COUNTS + 4
    .equ A_BLANK, A_COUNTS + 8
    .equ A_ADD, A_COUNTS + 12

    .global Convert_MapChunks
    .type Convert_MapChunks, %function
Convert_MapChunks:
    push    {r4-r11, lr}
    sub     sp, sp, #12
    mov     r12, r0                 @ todo and src change places
    mov     r0, r3
    mov     r3, r12
    sub     r1, r1, r0
    sub     r1, r1, #4
    cmp     r2, #0
    subne   r2, r2, r0
    subne   r2, r2, #4
    ldr     r5, =0x03ff03ff
    ldr     r6, =0x0c000c00
    ldr     r7, =0x70007000
    ldr     r8, =0x00010001
    ldr     r9, [sp, #A_ADD]
    orr     r9, r9, r9, lsl #16
    ldr     r10, [sp, #A_BLANK]
    orr     r10, r10, r10, lsl #16
    ldr     r11, [sp, #A_COUNTS]
    mov     r4, #0
10: @ the next chunk
    cmp     r3, #0
    beq     90f
    tst     r3, #1
    bne     20f
    mov     r3, r3, lsr #1          @ not this one
    add     r0, r0, #32
    add     r11, r11, #1
    eor     r4, r4, #2
    b       10b
20:
    stmia   sp, {r3, r4, r11}
    mov     r3, #0
    mov     r4, #8
    cmp     r2, #0
    bne     50f
40: @ a pair, with no hi
    ldr     r11, [r0], #4
    and     r12, r8, r11, lsr #13   @ the priority bit, in each half
    add     r3, r3, r12
    and     r12, r5, r11            @ the tile,
    and     lr, r7, r11, lsl #2
    orr     r12, r12, lr            @ and the palette
    add     r12, r12, r9
    bic     r12, r12, r6            @ (the tile's carry)
    and     r11, r6, r11, lsr #4    @ and the flips
    orr     r12, r12, r11
    str     r12, [r0, r1]
    subs    r4, r4, #1
    bne     40b
    b       30f
50: @ a pair, with hi
    ldr     r11, [r0], #4
    and     r12, r8, r11, lsr #13
    add     r3, r3, r12
    and     r12, r5, r11
    and     lr, r7, r11, lsl #2
    orr     r12, r12, lr
    add     r12, r12, r9
    bic     r12, r12, r6
    and     lr, r8, r11, lsr #13
    and     r11, r6, r11, lsr #4
    orr     r12, r12, r11           @ the converted entries
    rsb     lr, lr, lr, lsl #16     @ halfwords of ones where there's the priority bit
    eor     r11, r12, r10
    and     r11, r11, lr            @ what hi and lo swap
    eor     lr, r11, r10
    eor     r12, r12, r11
    str     lr, [r0, r2]
    str     r12, [r0, r1]
    subs    r4, r4, #1
    bne     50b
30: @ the chunk's count, in r3's halves
    mov     r12, r3, lsr #16
    mov     r3, r3, lsl #16
    add     r12, r12, r3, lsr #16
    ldr     r11, [sp, #8]
    ldrb    lr, [r11]
    strb    r12, [r11]
    sub     r12, r12, lr
    ldr     r4, [sp, #4]
    ldr     lr, [sp, #A_COLS]
    add     lr, lr, r4              @ cols[c & 1]
    ldrh    r3, [lr]
    add     r3, r3, r12
    strh    r3, [lr]
    ldmia   sp, {r3, r4, r11}
    mov     r3, r3, lsr #1
    add     r11, r11, #1
    eor     r4, r4, #2
    b       10b
90:
    add     sp, sp, #12
    pop     {r4-r11, lr}
    bx      lr
    .ltorg
    .size Convert_MapChunks, . - Convert_MapChunks
