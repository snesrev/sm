// See include/assert.h.
#include <stdio.h>

void __assert_fail(const char *expr, const char *file, int line) {
  printf("%s:%d: assertion failed: %s\n", file, line, expr);
}
