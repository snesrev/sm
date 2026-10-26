# Game Boy Advance build of the game. (The PC build is the Makefile.)
#   cmake -B build-gba -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake \
#       -DCMAKE_BUILD_TYPE=Release
#   cmake --build build-gba
# Produces build-gba/sm-engine.gba, the engine, and from it and sm.smc (if
# there), build-gba/sm.gba, the cartridge: see tools/make_cartridge.py.
cmake_minimum_required(VERSION 3.20)
project(sm_gba LANGUAGES C ASM)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Generic")
    message(FATAL_ERROR "Configure with -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake")
endif()
if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "The GBA build requires Clang. Found: ${CMAKE_C_COMPILER_ID}")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)

# Without the ROM (as in CI), the build makes only the engine; see
# tools/make_cartridge.py. SM_ROM is the ROM, by any name (or a zip with it in),
# or a directory it is in, found by its checksum: by default, this one.
set(SM_ROM "${CMAKE_SOURCE_DIR}" CACHE PATH
    "Your Super Metroid ROM, or a directory with it (sha1 da957f0d63d14cb441d215462904c4fa8519c613)")
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(SM_ROM_FILE "")
if(EXISTS "${SM_ROM}")
    # Which file it is, and what is wrong with it, now, not after the whole build.
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/make_cartridge.py --find ${SM_ROM}
        RESULT_VARIABLE rom_find_result
        OUTPUT_VARIABLE SM_ROM_FILE
        ERROR_VARIABLE rom_find_error
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(rom_find_result EQUAL 0)
        message(STATUS "SM_ROM: ${SM_ROM_FILE}")
    elseif(SM_ROM STREQUAL "${CMAKE_SOURCE_DIR}")
        set(SM_ROM_FILE "")
        message(STATUS "SM_ROM: no Super Metroid ROM here: building only sm-engine.gba. For the "
                       "cartridge, put your ROM here (any name), or pass -DSM_ROM=<its path, or a "
                       "directory with it>, and configure again.")
    else()
        message(FATAL_ERROR "${rom_find_error}")
    endif()
else()
    message(FATAL_ERROR "SM_ROM: there is no file or directory ${SM_ROM}")
endif()

# The code is not compiled -ffreestanding: that implies -fno-builtin, which
# stops clang inlining memcpy/memset and similar. Instead it links -nostdlib
# and gba/libc provides the few library functions needed; only that code is
# built with -fno-builtin. The game casts pointers between types freely, so
# no strict aliasing (the PC build doesn't either). A section a function and
# a variable, so that --gc-sections drops those unused (a fifth of the
# engine), not just unused files.
add_compile_options(${GBA_COMPILE_OPTIONS} -fno-strict-aliasing -ffunction-sections -fdata-sections)
add_link_options(${GBA_LINK_OPTIONS})
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/gba/libc/include)
include(cmake/compiler-rt.cmake)

add_subdirectory(gba)
