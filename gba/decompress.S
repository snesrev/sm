@ The game's decompressor (DecompressToMem in src/sm_80.c), by hand, in IWRAM.
@ A door's room load is some 34 KB of output, and the C version took 62
@ cycles a byte, as it works out the cartridge address of every byte it reads;
@ this takes 13.
@
@     uint8_t *Decompress(const uint8_t *src, uint8_t *dst);
@
@ Decompresses the data at |src| (in the cartridge, in order: see RomPtr) to
@ |dst|, and returns the end of its output. The data is commands, each with a
@ kind (the top 3 bits of its first byte) and a length less 1 (the low 5 bits).
@ A first byte of 111 is a long command: its kind is the next 3 bits, and its
@ length less 1 those below and the next byte, 10 bits. 0xff ends the data.
@ The kinds:
@     0   that many bytes from the data
@     1   one byte from the data, repeated
@     2   two, alternating
@     3   one byte, then each one more than the last
@     4-7 bytes from |dst|: from the 16-bit offset the data gives (4, 5), or
@         from that many bytes before the output's end (an 8-bit one: 6, 7);
@         kinds 5 and 7 xor them with 0xff
@ A copy may run into its own output, so it goes a byte at a time.

    .syntax unified
    .arm
    .section .iwram.text.decomp, "ax", %progbits
    .p2align 2

    .global Decompress
    .type Decompress, %function
Decompress:
    push    {r4-r7, lr}
    mov     r2, r1                  @ where the next byte goes
    b       next

raw:  @ Kind 0: bytes from the data.
    ldrb    r5, [r0], #1
    strb    r5, [r2], #1
    subs    r4, r4, #1              @ (r4 is the length less 1)
    bpl     raw

next:
    ldrb    r3, [r0], #1
    and     r4, r3, #0x1f
    mov     r6, r3, lsr #5
    cmp     r6, #7
    beq     long
dispatch:
    cmp     r6, #1
    blo     raw
    beq     fill1
    cmp     r6, #3
    blo     fill2
    beq     inc

    @ Kinds 4-7: bytes from |dst|.
    ldrb    r5, [r0], #1
    cmp     r6, #6
    subhs   r3, r2, r5              @ 6, 7: that many before
    ldrblo  r3, [r0], #1            @ 4, 5: by a 16-bit offset from |dst|
    orrlo   r5, r5, r3, lsl #8
    addlo   r3, r1, r5
    and     r7, r6, #1              @ 5, 7: xor 0xff
    rsb     r7, r7, r7, lsl #8
1:  ldrb    r5, [r3], #1
    eor     r5, r5, r7
    strb    r5, [r2], #1
    subs    r4, r4, #1
    bpl     1b
    b       next

long:
    cmp     r3, #0xff
    beq     done
    ldrb    r5, [r0], #1
    and     r4, r3, #3
    orr     r4, r5, r4, lsl #8
    and     r6, r3, #0x1c
    mov     r6, r6, lsr #2
    b       dispatch

done:
    mov     r0, r2
    pop     {r4-r7, lr}
    bx      lr

fill1:
    ldrb    r5, [r0], #1
    mov     r6, r5
    b       fill
fill2:
    ldrb    r5, [r0], #1
    ldrb    r6, [r0], #1
fill:
    strb    r5, [r2], #1
    subs    r4, r4, #1
    strbpl  r6, [r2], #1
    subspl  r4, r4, #1
    bpl     fill
    b       next

inc:
    ldrb    r5, [r0], #1
1:  strb    r5, [r2], #1
    add     r5, r5, #1
    subs    r4, r4, #1
    bpl     1b
    b       next
