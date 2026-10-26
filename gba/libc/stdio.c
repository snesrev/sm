// See include/stdio.h. Formatting supports what the game's messages use:
// %d %i %u %x %X %c %s %p %%, with flags '-' and '0', a width, a precision
// and the h, l, ll and z length modifiers. No floating point.
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// mGBA's debug log: https://github.com/mgba-emu/mgba/blob/master/include/mgba/internal/gba/io.h
#define MGBA_LOG_STRING ((volatile char *)0x04FFF600)
#define MGBA_LOG_FLAGS (*(volatile uint16_t *)0x04FFF700)
#define MGBA_LOG_ENABLE (*(volatile uint16_t *)0x04FFF780)
enum {
  kMgbaLogMax = 0x100,
  kMgbaLevelInfo = 3,
  kMgbaLogSend = 0x100,
};

static char line_buf[kMgbaLogMax];
static size_t line_len;
static int log_state;  // 0: not tried, 1: mGBA is listening, -1: it isn't

static void FlushLine(void) {
  if (log_state == 0) {
    MGBA_LOG_ENABLE = 0xC0DE;
    log_state = MGBA_LOG_ENABLE == 0x1DEA ? 1 : -1;
  }
  if (log_state > 0) {
    for (size_t i = 0; i < line_len; i++)
      MGBA_LOG_STRING[i] = line_buf[i];
    MGBA_LOG_STRING[line_len] = 0;
    MGBA_LOG_FLAGS = kMgbaLevelInfo | kMgbaLogSend;
  }
  line_len = 0;
}

int putchar(int c) {
  if (c == '\n' || line_len == sizeof(line_buf) - 1)
    FlushLine();
  if (c != '\n')
    line_buf[line_len++] = (char)c;
  return (unsigned char)c;
}

int puts(const char *s) {
  while (*s)
    putchar(*s++);
  putchar('\n');
  return 0;
}

typedef struct Out {
  char *buf;
  size_t size, len;
} Out;

static void Emit(Out *o, char c) {
  if (o->len + 1 < o->size)
    o->buf[o->len] = c;
  o->len++;
}

static void EmitPadded(Out *o, const char *s, size_t n, int width, bool left, char pad) {
  int fill = width > (int)n ? width - (int)n : 0;
  if (!left) {
    // Zero padding goes after a sign.
    if (pad == '0' && n && *s == '-') {
      Emit(o, *s++);
      n--;
    }
    while (fill--)
      Emit(o, pad);
  }
  while (n--)
    Emit(o, *s++);
  if (left) {
    while (fill-- > 0)
      Emit(o, ' ');
  }
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap) {
  Out o = {buf, size, 0};
  for (; *fmt; fmt++) {
    if (*fmt != '%') {
      Emit(&o, *fmt);
      continue;
    }
    fmt++;
    bool left = false;
    char pad = ' ';
    for (;; fmt++) {
      if (*fmt == '-')
        left = true;
      else if (*fmt == '0')
        pad = '0';
      else if (*fmt != ' ' && *fmt != '+' && *fmt != '#')
        break;
    }
    int width = 0, precision = -1;
    if (*fmt == '*') {
      width = va_arg(ap, int);
      fmt++;
    }
    while (*fmt >= '0' && *fmt <= '9')
      width = width * 10 + (*fmt++ - '0');
    if (*fmt == '.') {
      precision = 0;
      fmt++;
      if (*fmt == '*') {
        precision = va_arg(ap, int);
        fmt++;
      }
      while (*fmt >= '0' && *fmt <= '9')
        precision = precision * 10 + (*fmt++ - '0');
    }
    int longs = 0;
    while (*fmt == 'l' || *fmt == 'h' || *fmt == 'z') {
      if (*fmt == 'l')
        longs++;
      fmt++;
    }
    char tmp[24];
    switch (*fmt) {
    case 'd': case 'i': case 'u': case 'x': case 'X': case 'p': {
      bool is_signed = *fmt == 'd' || *fmt == 'i';
      unsigned base = (*fmt == 'u' || is_signed) ? 10 : 16;
      const char *digits = *fmt == 'X' ? "0123456789ABCDEF" : "0123456789abcdef";
      // 32-bit division only: ARM code mustn't call the 64-bit helpers.
      uint32_t v;
      bool neg = false;
      if (*fmt == 'p') {
        v = (uint32_t)(uintptr_t)va_arg(ap, void *);
      } else if (longs >= 2) {
        int64_t x = va_arg(ap, int64_t);
        v = (uint32_t)x;
        if (is_signed && x < 0)
          v = (uint32_t)-x, neg = true;
      } else if (is_signed) {
        int32_t x = va_arg(ap, int32_t);
        v = x < 0 ? (uint32_t)-x : (uint32_t)x;
        neg = x < 0;
      } else {
        v = va_arg(ap, uint32_t);
      }
      char *p = tmp + sizeof(tmp);
      int n = 0;
      do {
        *--p = digits[v % base];
        v /= base;
        n++;
      } while (v);
      while (n < precision) {
        *--p = '0';
        n++;
      }
      if (neg)
        *--p = '-';
      EmitPadded(&o, p, tmp + sizeof(tmp) - p, width, left, precision >= 0 ? ' ' : pad);
      break;
    }
    case 'c':
      tmp[0] = (char)va_arg(ap, int);
      EmitPadded(&o, tmp, 1, width, left, ' ');
      break;
    case 's': {
      const char *s = va_arg(ap, const char *);
      if (!s)
        s = "(null)";
      size_t n = strlen(s);
      if (precision >= 0 && (size_t)precision < n)
        n = precision;
      EmitPadded(&o, s, n, width, left, ' ');
      break;
    }
    case '%':
      Emit(&o, '%');
      break;
    default:
      Emit(&o, '%');
      if (!*fmt)
        fmt--;
      else
        Emit(&o, *fmt);
      break;
    }
  }
  if (size)
    buf[o.len < size ? o.len : size - 1] = 0;
  return (int)o.len;
}

int vprintf(const char *fmt, va_list ap) {
  char buf[kMgbaLogMax];
  int n = vsnprintf(buf, sizeof(buf), fmt, ap);
  for (const char *p = buf; *p; p++)
    putchar(*p);
  return n;
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = vprintf(fmt, ap);
  va_end(ap);
  return n;
}
