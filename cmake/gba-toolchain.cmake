# Cross-compile for the Game Boy Advance (ARM7TDMI, ARMv4T) with clang + lld.
#   cmake -B build-gba -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(GBA_TRIPLE armv4t-none-eabi)
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${GBA_TRIPLE})
set(CMAKE_ASM_COMPILER_TARGET ${GBA_TRIPLE})

find_program(CMAKE_OBJCOPY NAMES llvm-objcopy REQUIRED)

# There is no sysroot: no libc and no crt0. Whatever runtime support we need
# lives in gba/.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Flags the build can't do without. CMakeLists.txt applies these with
# add_compile_options()/add_link_options() rather than through
# CMAKE_<LANG>_FLAGS_INIT, so that setting CMAKE_C_FLAGS (to add -g, say)
# can't silently drop them. Without -mcpu, lld assumes ARMv5 and emits
# interworking thunks that don't work on the ARMv4T GBA.
#
# C is Thumb by default (it runs from the cartridge's 16-bit bus); hot code
# in *.iwram.c is ARM. Assembly states its own instruction set.
set(GBA_COMPILE_OPTIONS
    -mcpu=arm7tdmi
    -DSM_GBA=1
    $<$<COMPILE_LANGUAGE:C>:-mthumb>
)
set(GBA_LINK_OPTIONS -fuse-ld=lld -nostdlib -static)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
