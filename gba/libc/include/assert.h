// Minimal C library for the GBA. A failed assertion is logged to mGBA's
// debug log, and the game carries on (as it does in the PC release build).
#pragma once

void __assert_fail(const char *expr, const char *file, int line);

#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#define assert(e) ((e) ? (void)0 : __assert_fail(#e, __FILE__, __LINE__))
#endif
