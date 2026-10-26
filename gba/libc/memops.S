@ memcpy and memset, which the game and the compiler call all the time (a
@ frame's mixer, the PPU's registers, the dirty bitmap): ARM code in IWRAM.
@ The compiler also calls the ARM run-time ABI's names for them, which are
@ these (but for memset's arguments' order). The rest of the C library is
@ in string.c.
@
@ Words with ldm and stm: an ARM7TDMI's stm takes a cycle a word and one more,
@ where a str takes two cycles each. From equally aligned pointers, 32 bytes at
@ a time; from pointers aligned alike by halfwords, halfwords; otherwise (or a
@ few bytes) bytes. memcpy's regions mustn't overlap (see memmove).

    .syntax unified
    .arm
    .section .iwram.text.libc, "ax", %progbits
    .p2align 2

@ void *memcpy(void *dst, const void *src, size_t n)
    .global memcpy
    .type memcpy, %function
    .global __aeabi_memcpy
    .type __aeabi_memcpy, %function
    .global __aeabi_memcpy4
    .type __aeabi_memcpy4, %function
    .global __aeabi_memcpy8
    .type __aeabi_memcpy8, %function
memcpy:
__aeabi_memcpy:
__aeabi_memcpy4:
__aeabi_memcpy8:
    mov     r12, r0                 @ the return value
    cmp     r2, #8
    blo     .Lcopy_bytes
    eor     r3, r0, r1
    tst     r3, #3
    bne     .Lcopy_unaligned
    tst     r0, #3
    beq     2f
1:  ldrb    r3, [r1], #1            @ up to a word boundary
    strb    r3, [r0], #1
    sub     r2, r2, #1
    tst     r0, #3
    bne     1b
2:  cmp     r2, #32
    blo     5f
    push    {r4-r10}
3:  ldmia   r1!, {r3-r10}
    stmia   r0!, {r3-r10}
    sub     r2, r2, #32
    cmp     r2, #32
    bhs     3b
    tst     r2, #16
    ldmne   r1!, {r3-r6}
    stmne   r0!, {r3-r6}
    tst     r2, #8
    ldmne   r1!, {r3, r4}
    stmne   r0!, {r3, r4}
    pop     {r4-r10}
    tst     r2, #4
    ldrne   r3, [r1], #4
    strne   r3, [r0], #4
    b       .Lcopy_tail
5:  subs    r2, r2, #4              @ less than 32 bytes: a word at a time
    ldrhs   r3, [r1], #4
    strhs   r3, [r0], #4
    bhs     5b
    add     r2, r2, #4
.Lcopy_tail:
    ands    r2, r2, #3
    beq     .Lcopy_done
.Lcopy_bytes:
    cmp     r2, #0
    beq     .Lcopy_done
6:  ldrb    r3, [r1], #1
    strb    r3, [r0], #1
    subs    r2, r2, #1
    bne     6b
.Lcopy_done:
    mov     r0, r12
    bx      lr
.Lcopy_unaligned:
    orr     r3, r0, r1
    tst     r3, #1
    bne     .Lcopy_bytes
7:  ldrh    r3, [r1], #2            @ both on halfwords
    strh    r3, [r0], #2
    sub     r2, r2, #2
    cmp     r2, #2
    bhs     7b
    b       .Lcopy_bytes

@ void *memset(void *dst, int c, size_t n)
@ The ABI's: __aeabi_memset(dst, n, c), and __aeabi_memclr(dst, n).
    .global memset
    .type memset, %function
    .global __aeabi_memset
    .type __aeabi_memset, %function
    .global __aeabi_memset4
    .type __aeabi_memset4, %function
    .global __aeabi_memset8
    .type __aeabi_memset8, %function
    .global __aeabi_memclr
    .type __aeabi_memclr, %function
    .global __aeabi_memclr4
    .type __aeabi_memclr4, %function
    .global __aeabi_memclr8
    .type __aeabi_memclr8, %function
__aeabi_memset:
__aeabi_memset4:
__aeabi_memset8:
    mov     r3, r1
    mov     r1, r2
    mov     r2, r3
    b       memset
__aeabi_memclr:
__aeabi_memclr4:
__aeabi_memclr8:
    mov     r2, r1
    mov     r1, #0
    b       .Lset
memset:
    and     r1, r1, #0xff
    orr     r1, r1, r1, lsl #8
    orr     r1, r1, r1, lsl #16     @ the byte in all four
.Lset:
    mov     r12, r0                 @ the return value
    cmp     r2, #8
    blo     .Lset_bytes
    tst     r0, #3
    beq     2f
1:  strb    r1, [r0], #1            @ up to a word boundary
    sub     r2, r2, #1
    tst     r0, #3
    bne     1b
2:  cmp     r2, #32
    blo     5f
    push    {r4-r9}
    mov     r3, r1
    mov     r4, r1
    mov     r5, r1
    mov     r6, r1
    mov     r7, r1
    mov     r8, r1
    mov     r9, r1
3:  stmia   r0!, {r1, r3-r9}
    sub     r2, r2, #32
    cmp     r2, #32
    bhs     3b
    tst     r2, #16
    stmne   r0!, {r1, r3-r5}
    tst     r2, #8
    stmne   r0!, {r1, r3}
    pop     {r4-r9}
    tst     r2, #4
    strne   r1, [r0], #4
    b       .Lset_tail
5:  subs    r2, r2, #4              @ less than 32 bytes: a word at a time
    strhs   r1, [r0], #4
    bhs     5b
    add     r2, r2, #4
.Lset_tail:
    ands    r2, r2, #3
    beq     .Lset_done
.Lset_bytes:
    cmp     r2, #0
    beq     .Lset_done
6:  strb    r1, [r0], #1
    subs    r2, r2, #1
    bne     6b
.Lset_done:
    mov     r0, r12
    bx      lr
