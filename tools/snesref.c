// snesref: what the SNES would show of a frame, for comparing with the GBA's.
//
//     snesref SYMS PREFIX OUT.png
//
// Reads the GBA's memory as smrun's DUMP wrote it (PREFIX.ewram and
// PREFIX.iwram), finds the SNES's PPU state in it by SYMS (`llvm-nm` of
// sm.elf): VRAM, CGRAM, OAM and the registers of each line, as
// gba/render.iwram.c captured them. Then draws the 256x224 frame with the
// PC build's PPU, src/snes/ppu.c, and writes it as a PNG. (But for a
// gradient HDMA makes of the fixed color: the lines' registers have each
// state's first line's color, as SnesHw_HdmaColdata keeps it.)
//
// Build:
//     clang -O2 -o snesref tools/snesref.c src/snes/ppu.c -Igba -lz
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "../src/snes/ppu.h"
#include "snes_hw.h"

bool g_new_ppu = true;

static uint8_t ewram[0x40000], iwram[0x8000];

static void load(const char* prefix, const char* ext, uint8_t* buf, size_t size) {
    char path[512];
    snprintf(path, sizeof path, "%s.%s", prefix, ext);
    FILE* f = fopen(path, "rb");
    if (!f || fread(buf, 1, size, f) != size) {
        fprintf(stderr, "can't read %s\n", path);
        exit(1);
    }
    fclose(f);
}

static void* sym(const char* syms, const char* name) {
    FILE* f = fopen(syms, "r");
    if (!f) {
        fprintf(stderr, "can't read %s\n", syms);
        exit(1);
    }
    char line[512], n[256], type;
    unsigned addr;
    while (fgets(line, sizeof line, f)) {
        if (sscanf(line, "%x %c %255s", &addr, &type, n) == 3 && !strcmp(n, name)) {
            fclose(f);
            if (addr >> 24 == 2)
                return &ewram[addr & 0x3ffff];
            if (addr >> 24 == 3)
                return &iwram[addr & 0x7fff];
        }
    }
    fprintf(stderr, "no symbol %s\n", name);
    exit(1);
}

static void write_word(Ppu* ppu, uint8_t adr, uint16_t v) {
    ppu_write(ppu, adr, v & 0xff);
    ppu_write(ppu, adr, v >> 8);
}

// Sets the PPU's registers to a line's: its state's, and its scrolling
// (BGnHOFS, then BGnVOFS).
static void set_regs(Ppu* ppu, const PpuRegs* r, uint8_t obsel, const uint16_t* scroll) {
    ppu_write(ppu, 0x00, r->inidisp);
    ppu_write(ppu, 0x01, obsel);
    ppu_write(ppu, 0x05, r->bgmode);
    ppu_write(ppu, 0x06, r->mosaic);
    for (int i = 0; i < 4; i++)
        ppu_write(ppu, 0x07 + i, r->bgsc[i]);
    ppu_write(ppu, 0x0b, r->bgnba[0]);
    ppu_write(ppu, 0x0c, r->bgnba[1]);
    for (int i = 0; i < 4; i++) {
        write_word(ppu, 0x0d + 2 * i, scroll[i]);
        write_word(ppu, 0x0e + 2 * i, scroll[4 + i]);
    }
    // Mode 7's scroll shares BG1's registers: rewrite BG1's after.
    write_word(ppu, 0x0d, r->m7hofs);
    write_word(ppu, 0x0e, r->m7vofs);
    ppu->bgLayer[0].hScroll = scroll[0];
    ppu->bgLayer[0].vScroll = scroll[4];
    ppu_write(ppu, 0x1a, r->m7sel);
    for (int i = 0; i < 6; i++)
        write_word(ppu, 0x1b + i, r->m7[i]);
    for (int i = 0; i < 3; i++)
        ppu_write(ppu, 0x23 + i, r->wsel[i]);
    for (int i = 0; i < 4; i++)
        ppu_write(ppu, 0x26 + i, r->wh[i]);
    ppu_write(ppu, 0x2a, r->wbglog);
    ppu_write(ppu, 0x2b, r->wobjlog);
    ppu_write(ppu, 0x2c, r->tm);
    ppu_write(ppu, 0x2d, r->ts);
    ppu_write(ppu, 0x2e, r->tmw);
    ppu_write(ppu, 0x2f, r->tsw);
    ppu_write(ppu, 0x30, r->cgwsel);
    ppu_write(ppu, 0x31, r->cgadsub);
    ppu_write(ppu, 0x32, 0x20 | r->coldata[0]);
    ppu_write(ppu, 0x32, 0x40 | r->coldata[1]);
    ppu_write(ppu, 0x32, 0x80 | r->coldata[2]);
    ppu_write(ppu, 0x33, r->setini);
}

static void put32(unsigned char* p, uint32_t v) {
    p[0] = v >> 24, p[1] = v >> 16, p[2] = v >> 8, p[3] = v;
}

static void chunk(FILE* f, const char* type, const unsigned char* data, uint32_t len) {
    unsigned char hdr[8], crc[4];
    put32(hdr, len);
    memcpy(hdr + 4, type, 4);
    fwrite(hdr, 1, 8, f);
    fwrite(data, 1, len, f);
    // (zlib's crc32 of a null buffer is 0, not the CRC so far: IEND has no data.)
    put32(crc, crc32(crc32(0, (const unsigned char*) type, 4), data ? data : (const unsigned char*) "", len));
    fwrite(crc, 1, 4, f);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s SYMS PREFIX OUT.png\n", argv[0]);
        return 2;
    }
    load(argv[2], "ewram", ewram, sizeof ewram);
    load(argv[2], "iwram", iwram, sizeof iwram);
    const uint16_t* vram = sym(argv[1], "g_vram");
    const uint16_t* cgram = sym(argv[1], "g_cgram");
    const uint8_t* oam = sym(argv[1], "g_oam");
    const SnesHw* hw = sym(argv[1], "g_snes_hw");
    const PpuRegs* states = sym(argv[1], "line_states");
    const uint8_t* line_state = sym(argv[1], "line_state");
    // The windows' edges, by line (see SnesHw_RunDisplay).
    const uint8_t* line_wh = sym(argv[1], "line_wh");
    // The scrolling, by line: BGnHOFS | BGnVOFS << 16.
    const uint32_t* line_ofs = sym(argv[1], "line_ofs");

    static Snes snes;
    Ppu* ppu = ppu_init(&snes);
    static uint32_t pixels[224][256];
    ppu_reset(ppu);
    PpuBeginDrawing(ppu, (uint8_t*) pixels, sizeof pixels[0], 0);
    memcpy(ppu->vram, vram, sizeof ppu->vram);
    memcpy(ppu->cgram, cgram, sizeof ppu->cgram);
    memcpy(ppu->oam, oam, 512);
    memcpy(ppu->highOam, oam + 512, 32);
    ppu->lastBrightnessMult = 0xff;
    ppu_runLine(ppu, 0);
    for (int y = 0; y < 224; y++) {
        uint16_t scroll[8];
        for (int i = 0; i < 4; i++) {
            scroll[i] = line_ofs[y * 4 + i] & 0xffff;
            scroll[4 + i] = line_ofs[y * 4 + i] >> 16;
        }
        set_regs(ppu, &states[line_state[y]], hw->obsel, scroll);
        for (int i = 0; i < 4; i++)
            ppu_write(ppu, 0x26 + i, line_wh[y * 4 + i]);
        ppu_runLine(ppu, y + 1);
    }

    static unsigned char raw[224 * (256 * 3 + 1)];
    for (int y = 0; y < 224; y++) {
        unsigned char* row = raw + y * (256 * 3 + 1);
        row[0] = 0;
        for (int x = 0; x < 256; x++) {
            uint32_t c = pixels[y][x];
            row[1 + 3 * x] = c >> 16, row[2 + 3 * x] = c >> 8, row[3 + 3 * x] = c;
        }
    }
    uLongf zlen = compressBound(sizeof raw);
    unsigned char* z = malloc(zlen);
    compress(z, &zlen, raw, sizeof raw);
    FILE* f = fopen(argv[3], "wb");
    fwrite("\x89PNG\r\n\x1a\n", 1, 8, f);
    unsigned char ihdr[13] = {0};
    put32(ihdr, 256);
    put32(ihdr + 4, 224);
    ihdr[8] = 8, ihdr[9] = 2;
    chunk(f, "IHDR", ihdr, 13);
    chunk(f, "IDAT", z, zlen);
    chunk(f, "IEND", NULL, 0);
    fclose(f);
    return 0;
}
