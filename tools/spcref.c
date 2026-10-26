// spcref: what the SNES would play, for comparing with the GBA's sound.
//
//     spcref sm.smc LOG FRAMES OUT.wav
//
// Replays what the game asked of its sound driver, as a cartridge built
// with -DSM_AUDIO_LOG=ON logs it ("audio: FRAME upload ROM_OFFSET" and
// "audio: FRAME port N VALUE" lines, from smrun), through the PC build's
// driver and emulated DSP, as src/sm_rtl.c's RtlRenderAudio does: a frame's
// writes, then a frame's sound. Writes FRAMES frames at 32000 Hz.
//
// Build:
//     clang -O2 -I. -o spcref tools/spcref.c src/spc_player.c src/snes/dsp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/spc_player.h"

static uint8_t rom[0x300000];

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "usage: %s sm.smc LOG FRAMES OUT.wav\n", argv[0]);
        return 2;
    }
    FILE* f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, size == sizeof rom + 512 ? 512 : 0, SEEK_SET);
    if (!f || fread(rom, 1, sizeof rom, f) != sizeof rom) {
        fprintf(stderr, "can't read %s\n", argv[1]);
        return 1;
    }
    fclose(f);
    FILE* log = fopen(argv[2], "r");
    int frames = atoi(argv[3]);
    FILE* out = fopen(argv[4], "wb");
    if (!log || !out) {
        fprintf(stderr, "can't open files\n");
        return 1;
    }

    SpcPlayer* p = SpcPlayer_Create();
    SpcPlayer_Initialize(p);
    char line[512];
    unsigned ev_frame = 0, a = 0, b = 0;
    char kind[16];
    bool have = false;
    fseek(out, 44, SEEK_SET);
    uint32_t bytes = 0;
    for (int frame = 1; frame <= frames; frame++) {
        for (;;) {
            if (!have) {
                if (!fgets(line, sizeof line, log))
                    break;
                const char* s = strstr(line, "audio: ");
                if (!s || sscanf(s, "audio: %u %15s %u %u", &ev_frame, kind, &a, &b) < 3)
                    continue;
                have = true;
            }
            if (ev_frame > (unsigned) frame)
                break;
            if (!strcmp(kind, "upload"))
                SpcPlayer_Upload(p, rom + a);
            else
                p->input_ports[a & 3] = b;
            have = false;
        }
        SpcPlayer_GenerateSamples(p);
        int16_t samples[534 * 2];
        dsp_getSamples(p->dsp, samples, 534);
        fwrite(samples, 4, 534, out);
        bytes += 534 * 4;
    }
    uint32_t rate = 32000, v = 36 + bytes;
    fseek(out, 0, SEEK_SET);
    fwrite("RIFF", 1, 4, out);
    fwrite(&v, 4, 1, out);
    fwrite("WAVEfmt ", 1, 8, out);
    uint32_t fmt[4] = {16, 1 | 2 << 16, rate, rate * 4};
    fwrite(fmt, 4, 4, out);
    uint16_t align[2] = {4, 16};
    fwrite(align, 2, 2, out);
    fwrite("data", 1, 4, out);
    fwrite(&bytes, 4, 1, out);
    fclose(out);
    return 0;
}
