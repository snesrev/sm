// The GBA cartridge header, 0x08000004-0x080000BF. crt0.S supplies the
// entry branch at 0x08000000, and gba.ld places this right after it.
// https://problemkaputt.de/gbatek.htm#gbacartridgeheader
#include <stdint.h>

typedef struct GbaHeader {
  uint8_t logo[156];         // 0x04: must match the BIOS's copy on hardware
  char title[12] __attribute__((nonstring));          // 0xA0
  char game_code[4] __attribute__((nonstring));        // 0xAC
  char maker_code[2] __attribute__((nonstring));       // 0xB0
  uint8_t fixed;             // 0xB2: must be 0x96
  uint8_t unit_code;         // 0xB3
  uint8_t device_type;       // 0xB4
  uint8_t reserved1[7];      // 0xB5
  uint8_t software_version;  // 0xBC
  uint8_t complement;        // 0xBD: checksum of 0xA0-0xBC
  uint8_t reserved2[2];      // 0xBE
} GbaHeader;
_Static_assert(sizeof(GbaHeader) == 0xC0 - 0x04, "GBA header size");

// The BIOS checks that the bytes 0xA0-0xBD sum to -0x19 (mod 256). Those
// are the title, game code, maker code and 0x96 here.
#define SUM_TITLE ('S' + 'U' + 'P' + 'E' + 'R' + 'M' + 'E' + 'T' + 'R' + 'O' + 'I' + 'D')
#define SUM_CODES ('A' + 'S' + 'M' + 'E' + '0' + '0')

// NOTE: the Nintendo logo is left zeroed. The real BIOS refuses to boot
// without it; emulators using an HLE BIOS (mGBA's default) don't check.
__attribute__((section(".gba_header"), used))
const GbaHeader header = {
  .title = "SUPERMETROID",
  .game_code = "ASME",
  .maker_code = "00",
  .fixed = 0x96,
  .complement = (uint8_t)-(SUM_TITLE + SUM_CODES + 0x96 + 0x19),
};

// mGBA picks the cartridge's save type from a string like this one: the
// game's 8 KB of battery-backed SNES SRAM lives in the GBA's SRAM.
__attribute__((section(".rodata.save_type"), aligned(4)))
const char save_type[] = "SRAM_V113";

// Who made the port: in the ELF's .comment, and, as .comment isn't loaded
// (objcopy leaves it out of sm-engine.gba), in the cartridge as well, so that
// `strings` finds it in sm.elf, sm-engine.gba and sm.gba.
#define PORT_CREDIT "GBA port by: Nick Desaulniers <nick.desaulniers@gmail.com>"
__asm__(".pushsection .comment,\"MS\",%progbits,1\n"
        ".asciz \"" PORT_CREDIT "\"\n"
        ".popsection");
__attribute__((section(".rodata.credit")))
const char port_credit[] = PORT_CREDIT;
