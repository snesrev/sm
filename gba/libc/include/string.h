// Minimal C library for the GBA: just what the game and the compiler use.
// Implemented in ../string.c.
#pragma once

#include <stddef.h>

void *memcpy(void *__restrict dst, const void *__restrict src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dst, int value, size_t n);
int memcmp(const void *a, const void *b, size_t n);
size_t strlen(const char *s);
char *strcpy(char *__restrict dst, const char *__restrict src);
int strcmp(const char *a, const char *b);
