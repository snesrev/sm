// Minimal C library for the GBA: output goes to mGBA's debug log, a line
// at a time. There are no files. Implemented in ../stdio.c.
#pragma once

#include <stdarg.h>
#include <stddef.h>

#define EOF (-1)

int printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int vprintf(const char *fmt, va_list ap);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
int puts(const char *s);
int putchar(int c);
