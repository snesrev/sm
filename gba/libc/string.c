// The C library functions the game and the compiler call. Clang emits
// calls to memcpy/memset/memmove on its own (for struct copies, zeroing, and
// recognized loops), so these must exist. (memcpy and memset, which are
// called all the time, are in IWRAM, in memops.S.)
//
// Built -fno-builtin, and the functions carry no_builtin too, so that the
// optimizer doesn't turn these loops back into calls to themselves; and
// noinline, for one copy of each, not one per __aeabi_ alias.
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define NO_BUILTIN __attribute__((no_builtin, noinline))

// For word-at-a-time access to memory of any type.
typedef uint32_t __attribute__((may_alias)) Word;

// Words too, as memcpy: the game moves rooms' data so, kilobytes of it
// (as the rooms load).
NO_BUILTIN void *memmove(void *dst, const void *src, size_t n) {
  unsigned char *d = dst;
  const unsigned char *s = src;
  int aligned = (((uintptr_t)d ^ (uintptr_t)s) & 3) == 0;
  if (d < s) {
    if (aligned) {
      for (; n && ((uintptr_t)d & 3); --n)
        *d++ = *s++;
      Word *dw = (Word *)d;
      const Word *sw = (const Word *)s;
      for (; n >= 4; n -= 4)
        *dw++ = *sw++;
      d = (unsigned char *)dw;
      s = (const unsigned char *)sw;
    }
    while (n--)
      *d++ = *s++;
  } else {
    d += n;
    s += n;
    if (aligned) {
      for (; n && ((uintptr_t)d & 3); --n)
        *--d = *--s;
      Word *dw = (Word *)d;
      const Word *sw = (const Word *)s;
      for (; n >= 4; n -= 4)
        *--dw = *--sw;
      d = (unsigned char *)dw;
      s = (const unsigned char *)sw;
    }
    while (n--)
      *--d = *--s;
  }
  return dst;
}

NO_BUILTIN int memcmp(const void *a, const void *b, size_t n) {
  const unsigned char *x = a, *y = b;
  for (; n; --n, ++x, ++y) {
    if (*x != *y)
      return *x - *y;
  }
  return 0;
}

NO_BUILTIN size_t strlen(const char *s) {
  const char *p = s;
  while (*p)
    ++p;
  return (size_t)(p - s);
}

NO_BUILTIN char *strcpy(char *__restrict dst, const char *__restrict src) {
  char *d = dst;
  while ((*d++ = *src++) != 0) {}
  return dst;
}

NO_BUILTIN int strcmp(const char *a, const char *b) {
  for (; *a && *a == *b; ++a, ++b) {}
  return (unsigned char)*a - (unsigned char)*b;
}

// ARM run-time ABI memory helpers, which clang emits instead of memmove on
// EABI targets. (memcpy's and memset's are in memops.S.) The 4/8 variants
// promise aligned pointers, which this simple version doesn't need.
// https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst
void __aeabi_memmove(void *dst, const void *src, size_t n) { memmove(dst, src, n); }
void __aeabi_memmove4(void *dst, const void *src, size_t n) { memmove(dst, src, n); }
void __aeabi_memmove8(void *dst, const void *src, size_t n) { memmove(dst, src, n); }
