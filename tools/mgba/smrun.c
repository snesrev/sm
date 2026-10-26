// smrun: runs the GBA cartridge in libmgba, headless, printing its debug
// log and saving screenshots.
//
//     smrun ROM FRAMES [SHOTS OUT_PREFIX]
//
// SHOTS is a comma-separated list of frame numbers (1-based) to save as
// OUT_PREFIX<frame>.png.
//
// With KEYS=FROM:STEP:K1,K2,... in the environment, it holds buttons: from
// frame FROM, K1 for STEP frames, then K2, and so on, round and round. Each
// K is buttons joined by '+' (A, B, SELECT, START, RIGHT, LEFT, UP, DOWN, R,
// L), or '-' for none.
//
// With AFTER=TEXT, KEYS and SHOTS count frames from the first log line
// containing TEXT (frame 1 is the frame after), and PRE_KEYS (in KEYS's
// form, counting from the start) holds buttons before then.
//
// With PROFILE=N SYMS=FILE, it runs the N frames from frame PROFILE_FROM
// (default 1, counted like SHOTS) an instruction at a time, and prints
// where their cycles went, by function (the PROFILE_TOP most, default
// 50): SYMS is `llvm-nm -n sm.elf`. With
// PCS=LO:HI (hex) too, it also prints the cycles and count of each
// instruction with an
// address in [LO, HI), to see where in a function they go.
//
// With TRAP0=1 too, if the CPU goes to address 0 or the cartridge's entry
// (a call through a null pointer, which resets the GBA), it prints the 64
// instructions before, with their functions and LR, and stops.
//
// With DUMP=FRAME:PREFIX, it writes the GBA's memory after that frame
// (counted like SHOTS) to PREFIX.ewram, PREFIX.iwram, .io, .pal, .vram and
// .oam: for tools/snesref.c, and for looking into.
//
// With SHOT_ON=TEXT, for each log line containing TEXT, it saves the
// picture two frames later (time enough for it to be on screen) as
// OUT_PREFIX<label>.png, and the memory as DUMP's does, to OUT_PREFIX<label>:
// the label is the word after TEXT (gba/tour.c holds its frames for this).
//
// With STOP_ON=TEXT, it stops (a few frames after the shot a line may ask
// for) at the first log line containing TEXT, before FRAMES.
//
// With FORK=ADDR:V1,V2,... (ADDR in hex, the Vs in decimal) as well, once
// SHOT_ON has saved its first picture it keeps the machine's whole state,
// and runs on from it once for each V, with V written as a 32-bit word at
// ADDR first, each run until its next picture: so each is independent of
// the others (and of which of them ran before), as a tour of the rooms that
// goes through them one after another isn't (see tools/room-tour.sh).
//
// With AUDIO=OUT.wav, it records the sound, at 32768 Hz.
// With MEMCHECK=FILE and SYMS, it prints each place in the game's code that
// loads or stores outside the memory it may use, once, with the address:
// FILE has lines "code LO HI" (the game's code: the PCs checked), "data LO
// HI" (what they may touch) and "helper LO HI" (memcpy and so on: checked
// when called from the game's code, by LR), in hex, [LO, HI).
// With STACKCHECK=1, it prints each new low of the stack pointer in IRQ mode
// and in System mode (the game's), seen at stores, with where (SYMS names it).
// With STATE=FILE (an mGBA save state, .ss1 and so on, of the same
// cartridge), it starts from that state instead of a reset; with
// STATE_OUT=FRAME:FILE, it saves one after that frame.
// With WATCH=LO:HI (hex) and SYMS, it prints the first write into [LO, HI)
// from each place in the code (the CPU's or a DMA's), with its frame, the
// function and its caller: to find a stray write.
//
// With SAVE=FILE.sav, the cartridge's SRAM starts as that file's (which is
// left as it was: the game's saves go to a copy in memory).
//
// Build with tools/mgba/build.sh, against mGBA 0.10.
#include <mgba/flags.h>
#include <mgba/core/config.h>
#include <mgba/core/core.h>
#include <mgba/core/blip_buf.h>
#include <mgba/core/log.h>
#include <mgba/core/serialize.h>
#include <mgba/core/timing.h>
#include <mgba/internal/arm/arm.h>
#include <mgba/internal/gba/gba.h>
#include <mgba-util/vfs.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static uint32_t parse_keys(const char* k, const char* end) {
    static const char* names[] = {"A", "B", "SELECT", "START", "RIGHT", "LEFT", "UP", "DOWN",
                                  "R", "L"};
    uint32_t keys = 0;
    while (k < end) {
        const char* plus = memchr(k, '+', end - k);
        const char* stop = plus ? plus : end;
        for (unsigned i = 0; i < sizeof names / sizeof *names; ++i) {
            if (strlen(names[i]) == (size_t) (stop - k) && !strncmp(names[i], k, stop - k))
                keys |= 1u << i;
        }
        k = plus ? plus + 1 : end;
    }
    return keys;
}

static int current_frame;
static const char* after_text;
static int after_frame;  // when after_text appeared, or 0
static const char* shot_text;
static int shot_frame;  // when to save SHOT_ON's next, or 0
static char shot_label[64];
static const char* stop_text;
static int stop_frame;  // when to stop, or 0

static void logger_fn(struct mLogger* logger, int category, enum mLogLevel level,
                      const char* format, va_list args) {
    (void) logger;
    const char* name = mLogCategoryName(category);
    if (level & (mLOG_FATAL | mLOG_ERROR) || (name && !strcmp(name, "GBA Debug")) ||
        (level & mLOG_WARN && name && strstr(name, "state"))) {
        char line[1024];
        vsnprintf(line, sizeof line, format, args);
        printf("[%d] %s\n", current_frame, line);
        fflush(stdout);
        if (after_text && !after_frame && strstr(line, after_text))
            after_frame = current_frame;
        if (stop_text && !stop_frame && strstr(line, stop_text))
            stop_frame = current_frame + 8;
        const char* shot = shot_text ? strstr(line, shot_text) : NULL;
        if (shot) {
            shot_frame = current_frame + 2;
            sscanf(shot + strlen(shot_text), " %63s", shot_label);
        }
    }
}

// FORK's: the state kept, the address and values, how many have run, and the
// frame the run began (it is given up after kForkMaxFrames).
enum {
    kForkMaxValues = 4096,
    kForkMaxFrames = 6000,
    kForkSaveFlags = SAVESTATE_SAVEDATA | SAVESTATE_RTC
};
static struct VFile* fork_state;
static uint32_t fork_addr, fork_values[kForkMaxValues];
static int fork_n, fork_next, fork_run_from;

// Runs from the kept state with the next value, or says there is none.
static bool fork_run(struct mCore* core, int frame) {
    if (fork_next >= fork_n)
        return false;
    fork_state->seek(fork_state, 0, SEEK_SET);
    if (!mCoreLoadStateNamed(core, fork_state, kForkSaveFlags)) {
        fprintf(stderr, "can't load the saved state\n");
        exit(1);
    }
    core->rawWrite32(core, fork_addr, -1, fork_values[fork_next]);
    shot_frame = 0;
    fork_run_from = frame;
    printf("[%d] fork: %u\n", frame, (unsigned) fork_values[fork_next++]);
    fflush(stdout);
    return true;
}

struct Keys {
    int from, step, n;
    uint32_t steps[4096];
};

static struct Keys parse_schedule(const char* spec) {
    struct Keys k = {.from = -1, .step = 1};
    if (!spec)
        return k;
    k.from = atoi(spec);
    const char* colon = strchr(spec, ':');
    k.step = colon ? atoi(colon + 1) : 1;
    const char* list = colon ? strchr(colon + 1, ':') : NULL;
    for (const char* p = list ? list + 1 : ""; *p && k.n < 4096;) {
        const char* comma = strchr(p, ',');
        const char* end = comma ? comma : p + strlen(p);
        k.steps[k.n++] = parse_keys(p, end);
        p = comma ? comma + 1 : end;
    }
    return k;
}

static uint32_t schedule_keys(const struct Keys* k, int frame) {
    if (!k->n || frame < k->from)
        return 0;
    return k->steps[((frame - k->from) / k->step) % k->n];
}

struct Sym {
    uint32_t addr;
    char name[96];
    unsigned long long cycles;
};
static uint32_t pcs_lo, pcs_hi;
static unsigned long long *pcs_cycles, *pcs_count;
static struct Sym* syms;
static int nsyms;

static void load_syms(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "can't read %s\n", path);
        exit(1);
    }
    char line[512];
    int cap = 4096;
    // Below every symbol: the BIOS, as in VBlankIntrWait's halt.
    syms = realloc(syms, cap * sizeof *syms);
    syms[nsyms].addr = 0;
    snprintf(syms[nsyms].name, sizeof syms[nsyms].name, "(bios)");
    syms[nsyms++].cycles = 0;
    while (fgets(line, sizeof line, f)) {
        unsigned addr;
        char type, name[256];
        if (sscanf(line, "%x %c %255s", &addr, &type, name) != 3 || (type != 't' && type != 'T'))
            continue;
        if (nsyms == cap)
            syms = realloc(syms, (cap = cap ? cap * 2 : 4096) * sizeof *syms);
        syms[nsyms].addr = addr & ~1u;
        snprintf(syms[nsyms].name, sizeof syms[nsyms].name, "%s", name);
        syms[nsyms].cycles = 0;
        nsyms++;
    }
    fclose(f);
}

static struct Sym* find_sym(uint32_t pc) {
    int lo = 0, hi = nsyms - 1;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (syms[mid].addr <= pc)
            lo = mid;
        else
            hi = mid - 1;
    }
    return &syms[lo];
}

static int by_cycles(const void* a, const void* b) {
    const struct Sym *x = a, *y = b;
    return x->cycles < y->cycles ? 1 : x->cycles > y->cycles ? -1 : 0;
}

// Runs a frame an instruction at a time, adding each one's cycles to its
// function's.
static unsigned long long profile_frame(struct mCore* core) {
    struct ARMCore* cpu = core->cpu;
    uint32_t start_frame = core->frameCounter(core);
    uint64_t last = mTimingGlobalTime(core->timing), start = last;
    // The last instructions run, for TRAP0.
    static uint32_t ring[64], ring_lr[64];
    static unsigned ring_at;
    while (core->frameCounter(core) == start_frame) {
        uint32_t pc = cpu->gprs[15] - (cpu->executionMode == MODE_ARM ? 4 : 2);
        if (getenv("TRAP0") && (pc == 0 || pc == 0x08000000)) {
            printf("trap: pc 0 at frame %u\n", core->frameCounter(core));
            for (unsigned k = 0; k < 64; k++) {
                uint32_t a = ring[(ring_at + k) % 64];
                printf("trap: %08x %s (lr %08x %s)\n", a, find_sym(a)->name,
                       ring_lr[(ring_at + k) % 64], find_sym(ring_lr[(ring_at + k) % 64])->name);
            }
            exit(1);
        }
        ring[ring_at % 64] = pc, ring_lr[ring_at % 64] = cpu->gprs[14], ring_at++;
        core->step(core);
        uint64_t now = mTimingGlobalTime(core->timing);
        find_sym(pc)->cycles += now - last;
        if (pcs_cycles && pc >= pcs_lo && pc < pcs_hi)
            pcs_cycles[(pc - pcs_lo) / 2] += now - last, pcs_count[(pc - pcs_lo) / 2]++;
        last = now;
    }
    return last - start;
}

// WATCH's hooks, on the CPU's stores.
static uint32_t watch_lo, watch_hi;
static struct ARMMemory watch_orig;
static uint32_t watch_seen[4096];
static unsigned watch_nseen;

static void watch_hit(struct ARMCore* cpu, uint32_t address, uint32_t size, uint32_t value) {
    if (address + size <= watch_lo || address >= watch_hi)
        return;
    uint32_t pc = cpu->gprs[15] - (cpu->executionMode == MODE_ARM ? 8 : 4);
    for (unsigned i = 0; i < watch_nseen; i++) {
        if (watch_seen[i] == pc)
            return;
    }
    if (watch_nseen < sizeof watch_seen / sizeof *watch_seen)
        watch_seen[watch_nseen++] = pc;
    uint32_t lr = cpu->gprs[14];
    printf("watch: frame %d write%u %08x = %x at %08x %s (lr %08x %s)\n", current_frame, size * 8,
           address, value, pc, nsyms ? find_sym(pc)->name : "", lr, nsyms ? find_sym(lr)->name : "");
}

static void watch_store32(struct ARMCore* cpu, uint32_t address, int32_t value, int* cycles) {
    watch_hit(cpu, address, 4, value);
    watch_orig.store32(cpu, address, value, cycles);
}

static void watch_store16(struct ARMCore* cpu, uint32_t address, int16_t value, int* cycles) {
    watch_hit(cpu, address, 2, (uint16_t)value);
    watch_orig.store16(cpu, address, value, cycles);
}

static void watch_store8(struct ARMCore* cpu, uint32_t address, int8_t value, int* cycles) {
    watch_hit(cpu, address, 1, (uint8_t)value);
    watch_orig.store8(cpu, address, value, cycles);
}

static uint32_t watch_store_multiple(struct ARMCore* cpu, uint32_t base, int mask,
                                     enum LSMDirection direction, int* cycles) {
    uint32_t n = __builtin_popcount(mask & 0xffff) * 4;
    uint32_t lo = direction & LSM_D ? base - n + (direction & LSM_B ? 0 : 4) : base + (direction & LSM_B ? 4 : 0);
    watch_hit(cpu, lo, n, 0);
    return watch_orig.storeMultiple(cpu, base, mask, direction, cycles);
}

// MEMCHECK's ranges and hooks (which go on top of WATCH's, if both).
struct Range {
    uint32_t lo, hi;
};
static struct Range mc_code[8192], mc_data[1024], mc_helper[64];
static unsigned mc_ncode, mc_ndata, mc_nhelper;
static struct ARMMemory mc_orig;
// The places already printed, a hash table of them (0 is empty).
static uint32_t mc_seen[1 << 16];

static int by_lo(const void* a, const void* b) {
    const struct Range *x = a, *y = b;
    return x->lo < y->lo ? -1 : x->lo > y->lo;
}

static bool in_ranges(const struct Range* r, unsigned n, uint32_t a) {
    unsigned lo = 0, hi = n;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        if (r[mid].hi <= a)
            lo = mid + 1;
        else if (r[mid].lo > a)
            hi = mid;
        else
            return true;
    }
    return false;
}

static void load_memcheck(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "smrun: can't read %s\n", path);
        exit(1);
    }
    char kind[16];
    unsigned lo, hi;
    while (fscanf(f, "%15s %x %x", kind, &lo, &hi) == 3) {
        if (!strcmp(kind, "code") && mc_ncode < 8192)
            mc_code[mc_ncode++] = (struct Range){lo, hi};
        else if (!strcmp(kind, "data") && mc_ndata < 1024)
            mc_data[mc_ndata++] = (struct Range){lo, hi};
        else if (!strcmp(kind, "helper") && mc_nhelper < 64)
            mc_helper[mc_nhelper++] = (struct Range){lo, hi};
    }
    fclose(f);
    qsort(mc_code, mc_ncode, sizeof *mc_code, by_lo);
    qsort(mc_data, mc_ndata, sizeof *mc_data, by_lo);
    qsort(mc_helper, mc_nhelper, sizeof *mc_helper, by_lo);
    // (Overlapping ones merged, for in_ranges: the cartridge's has the game's tables in it.)
    unsigned n = 0;
    for (unsigned i = 0; i < mc_ndata; i++) {
        if (n && mc_data[i].lo <= mc_data[n - 1].hi) {
            if (mc_data[i].hi > mc_data[n - 1].hi)
                mc_data[n - 1].hi = mc_data[i].hi;
        } else {
            mc_data[n++] = mc_data[i];
        }
    }
    mc_ndata = n;
}

static void mc_check(struct ARMCore* cpu, uint32_t address, uint32_t size, const char* what) {
    uint32_t pc = cpu->gprs[15] - (cpu->executionMode == MODE_ARM ? 8 : 4);
    uint32_t lr = cpu->gprs[14];
    // (Not the hardware's registers and memory: the game's code goes through the port's
    // functions for those, and the DMAs, which these hooks see too, write there.)
    if (address - 0x04000000u < 0x04000000u)
        return;
    // (Nor a DMA's reads and writes, made in the game's code's time.)
    if (((struct GBA*)cpu->master)->cpuBlocked)
        return;
    bool game = in_ranges(mc_code, mc_ncode, pc);
    if (!game && in_ranges(mc_helper, mc_nhelper, pc) && in_ranges(mc_code, mc_ncode, lr & ~1u))
        game = true;
    if (!game || in_ranges(mc_data, mc_ndata, address) ||
        (size > 1 && in_ranges(mc_data, mc_ndata, address + size - 1)))
        return;
    // (Seen by place and kind: a place's first of each.)
    uint32_t key = pc ^ (what[0] == 's') << 31, h = (key * 2654435761u) >> 16;
    while (mc_seen[h] && mc_seen[h] != key)
        h = (h + 1) & 0xffff;
    if (mc_seen[h])
        return;
    mc_seen[h] = key;
    printf("memcheck: frame %d %s%u %08x (%s) at %08x %s (lr %08x %s)\n", current_frame, what,
           size * 8, address, nsyms ? find_sym(address)->name : "", pc,
           nsyms ? find_sym(pc)->name : "", lr, nsyms ? find_sym(lr)->name : "");
}

static uint32_t mc_load32(struct ARMCore* cpu, uint32_t address, int* cycles) {
    mc_check(cpu, address, 4, "load");
    return mc_orig.load32(cpu, address, cycles);
}

static uint32_t mc_load16(struct ARMCore* cpu, uint32_t address, int* cycles) {
    mc_check(cpu, address, 2, "load");
    return mc_orig.load16(cpu, address, cycles);
}

static uint32_t mc_load8(struct ARMCore* cpu, uint32_t address, int* cycles) {
    mc_check(cpu, address, 1, "load");
    return mc_orig.load8(cpu, address, cycles);
}

static void mc_store32(struct ARMCore* cpu, uint32_t address, int32_t value, int* cycles) {
    mc_check(cpu, address, 4, "store");
    mc_orig.store32(cpu, address, value, cycles);
}

static void mc_store16(struct ARMCore* cpu, uint32_t address, int16_t value, int* cycles) {
    mc_check(cpu, address, 2, "store");
    mc_orig.store16(cpu, address, value, cycles);
}

static void mc_store8(struct ARMCore* cpu, uint32_t address, int8_t value, int* cycles) {
    mc_check(cpu, address, 1, "store");
    mc_orig.store8(cpu, address, value, cycles);
}

// The lowest address of a load or store multiple.
static uint32_t lsm_lo(uint32_t base, int mask, enum LSMDirection direction, uint32_t* n) {
    *n = __builtin_popcount(mask & 0xffff) * 4;
    return direction & LSM_D ? base - *n + (direction & LSM_B ? 0 : 4) : base + (direction & LSM_B ? 4 : 0);
}

static uint32_t mc_load_multiple(struct ARMCore* cpu, uint32_t base, int mask,
                                 enum LSMDirection direction, int* cycles) {
    uint32_t n, lo = lsm_lo(base, mask, direction, &n);
    mc_check(cpu, lo, n, "load");
    return mc_orig.loadMultiple(cpu, base, mask, direction, cycles);
}

static uint32_t mc_store_multiple(struct ARMCore* cpu, uint32_t base, int mask,
                                  enum LSMDirection direction, int* cycles) {
    uint32_t n, lo = lsm_lo(base, mask, direction, &n);
    mc_check(cpu, lo, n, "store");
    return mc_orig.storeMultiple(cpu, base, mask, direction, cycles);
}

// STACKCHECK's hooks (on top of the others').
static struct ARMMemory sc_orig;
static uint32_t sc_low_irq = 0xffffffff, sc_low_sys = 0xffffffff;

static void sc_check(struct ARMCore* cpu) {
    uint32_t sp = cpu->gprs[13];
    uint32_t* low = cpu->privilegeMode == MODE_IRQ ? &sc_low_irq
                    : cpu->privilegeMode == MODE_SYSTEM ? &sc_low_sys : NULL;
    if (!low || sp >= *low)
        return;
    *low = sp;
    uint32_t pc = cpu->gprs[15] - (cpu->executionMode == MODE_ARM ? 8 : 4);
    printf("stackcheck: frame %d %s sp %08x at %08x %s (lr %08x %s)\n", current_frame,
           cpu->privilegeMode == MODE_IRQ ? "irq" : "sys", sp, pc, nsyms ? find_sym(pc)->name : "",
           cpu->gprs[14], nsyms ? find_sym(cpu->gprs[14])->name : "");
}

static void sc_store32(struct ARMCore* cpu, uint32_t address, int32_t value, int* cycles) {
    sc_check(cpu);
    sc_orig.store32(cpu, address, value, cycles);
}

static void sc_store16(struct ARMCore* cpu, uint32_t address, int16_t value, int* cycles) {
    sc_check(cpu);
    sc_orig.store16(cpu, address, value, cycles);
}

static void sc_store8(struct ARMCore* cpu, uint32_t address, int8_t value, int* cycles) {
    sc_check(cpu);
    sc_orig.store8(cpu, address, value, cycles);
}

static uint32_t sc_store_multiple(struct ARMCore* cpu, uint32_t base, int mask,
                                  enum LSMDirection direction, int* cycles) {
    uint32_t r = sc_orig.storeMultiple(cpu, base, mask, direction, cycles);
    sc_check(cpu);  // (after: a push moves sp first)
    return r;
}

static void dump_memory(struct mCore* core, const char* prefix) {
    static const struct { const char* ext; uint32_t base, size; } kRegions[] = {
        {"ewram", 0x02000000, 0x40000},
        {"iwram", 0x03000000, 0x8000},
        {"io", 0x04000000, 0x400},
        {"pal", 0x05000000, 0x400},
        {"vram", 0x06000000, 0x18000},
        {"oam", 0x07000000, 0x400},
    };
    for (unsigned r = 0; r < sizeof kRegions / sizeof *kRegions; r++) {
        char path[512];
        snprintf(path, sizeof path, "%s.%s", prefix, kRegions[r].ext);
        FILE* f = fopen(path, "wb");
        for (uint32_t a = 0; a < kRegions[r].size; a++)
            fputc(core->rawRead8(core, kRegions[r].base + a, -1), f);
        fclose(f);
    }
}

static FILE* audio_out;
static uint32_t audio_bytes;

static void write_wav_header(FILE* f, uint32_t bytes) {
    uint32_t rate = 32768;
    fseek(f, 0, SEEK_SET);
    fwrite("RIFF", 1, 4, f);
    uint32_t v = 36 + bytes;
    fwrite(&v, 4, 1, f);
    fwrite("WAVEfmt ", 1, 8, f);
    uint32_t fmt[4] = {16, 1 | 2 << 16, rate, rate * 4};
    fwrite(fmt, 4, 4, f);
    uint16_t align[2] = {4, 16};
    fwrite(align, 2, 2, f);
    fwrite("data", 1, 4, f);
    fwrite(&bytes, 4, 1, f);
}

static void record_audio(struct mCore* core) {
    blip_t* left = core->getAudioChannel(core, 0);
    blip_t* right = core->getAudioChannel(core, 1);
    int available = blip_samples_avail(left);
    static short buf[2 * 8192];
    while (available > 0) {
        int n = available > 8192 ? 8192 : available;
        blip_read_samples(left, buf, n, 1);
        blip_read_samples(right, buf + 1, n, 1);
        fwrite(buf, 4, n, audio_out);
        audio_bytes += 4 * n;
        available -= n;
    }
}

static void put32(unsigned char* p, uint32_t v) {
    p[0] = v >> 24, p[1] = v >> 16, p[2] = v >> 8, p[3] = v;
}

static void chunk(FILE* f, const char* type, const unsigned char* data, uint32_t len) {
    unsigned char hdr[8];
    put32(hdr, len);
    memcpy(hdr + 4, type, 4);
    fwrite(hdr, 1, 8, f);
    fwrite(data, 1, len, f);
    // (zlib's crc32 of a null buffer is 0, not the CRC so far: IEND has no data.)
    uint32_t crc = crc32(crc32(0, (const unsigned char*) type, 4), data ? data : (const unsigned char*) "", len);
    unsigned char c[4];
    put32(c, crc);
    fwrite(c, 1, 4, f);
}

static void write_png(const char* path, const color_t* pixels, unsigned w, unsigned h, unsigned stride) {
    size_t raw_len = (size_t) h * (w * 3 + 1);
    unsigned char* raw = malloc(raw_len);
    for (unsigned y = 0; y < h; ++y) {
        unsigned char* row = raw + y * (w * 3 + 1);
        row[0] = 0;
        for (unsigned x = 0; x < w; ++x) {
            uint32_t c = pixels[y * stride + x];
            row[1 + x * 3] = c & 0xff;
            row[2 + x * 3] = (c >> 8) & 0xff;
            row[3 + x * 3] = (c >> 16) & 0xff;
        }
    }
    uLongf zlen = compressBound(raw_len);
    unsigned char* z = malloc(zlen);
    compress(z, &zlen, raw, raw_len);
    FILE* f = fopen(path, "wb");
    fwrite("\x89PNG\r\n\x1a\n", 1, 8, f);
    unsigned char ihdr[13];
    put32(ihdr, w);
    put32(ihdr + 4, h);
    ihdr[8] = 8, ihdr[9] = 2, ihdr[10] = 0, ihdr[11] = 0, ihdr[12] = 0;
    chunk(f, "IHDR", ihdr, 13);
    chunk(f, "IDAT", z, zlen);
    chunk(f, "IEND", NULL, 0);
    fclose(f);
    free(raw);
    free(z);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s ROM FRAMES [SHOTS OUT_PREFIX]\n", argv[0]);
        return 2;
    }
    static struct mLogger logger = {.log = logger_fn};
    mLogSetDefaultLogger(&logger);
    struct mCore* core = mCoreFind(argv[1]);
    if (!core || !core->init(core)) {
        fprintf(stderr, "%s: can't open %s\n", argv[0], argv[1]);
        return 1;
    }
    mCoreInitConfig(core, NULL);
    unsigned width, height;
    core->desiredVideoDimensions(core, &width, &height);
    color_t* pixels = calloc(width * height, sizeof(color_t));
    core->setVideoBuffer(core, pixels, width);
    if (!mCoreLoadFile(core, argv[1])) {
        fprintf(stderr, "%s: can't load %s\n", argv[0], argv[1]);
        return 1;
    }
    if (getenv("SAVE")) {
        struct VFile* vf = VFileOpen(getenv("SAVE"), O_RDONLY);
        if (!vf || !core->loadTemporarySave(core, vf)) {
            fprintf(stderr, "%s: can't load %s\n", argv[0], getenv("SAVE"));
            return 1;
        }
    }
    core->reset(core);
    if (getenv("STATE")) {
        struct VFile* vf = VFileOpen(getenv("STATE"), O_RDONLY);
        if (!vf || !mCoreLoadStateNamed(core, vf, 0)) {
            fprintf(stderr, "%s: can't load the state %s\n", argv[0], getenv("STATE"));
            return 1;
        }
        vf->close(vf);
    }

    struct Keys pre = parse_schedule(getenv("PRE_KEYS"));
    struct Keys keys = parse_schedule(getenv("KEYS"));
    after_text = getenv("AFTER");
    shot_text = getenv("SHOT_ON");
    stop_text = getenv("STOP_ON");
    if (getenv("AUDIO")) {
        audio_out = fopen(getenv("AUDIO"), "wb");
        write_wav_header(audio_out, 0);
        core->setAudioBufferSize(core, 8192);
        blip_set_rates(core->getAudioChannel(core, 0), core->frequency(core), 32768);
        blip_set_rates(core->getAudioChannel(core, 1), core->frequency(core), 32768);
    }
    if (getenv("WATCH")) {
        watch_lo = strtoul(getenv("WATCH"), NULL, 16);
        const char* colon = strchr(getenv("WATCH"), ':');
        watch_hi = colon ? strtoul(colon + 1, NULL, 16) : watch_lo + 1;
        if (getenv("SYMS"))
            load_syms(getenv("SYMS"));
        struct ARMCore* cpu = core->cpu;
        watch_orig = cpu->memory;
        cpu->memory.store32 = watch_store32;
        cpu->memory.store16 = watch_store16;
        cpu->memory.store8 = watch_store8;
        cpu->memory.storeMultiple = watch_store_multiple;
    }
    if (getenv("MEMCHECK")) {
        load_memcheck(getenv("MEMCHECK"));
        if (getenv("SYMS") && !nsyms)
            load_syms(getenv("SYMS"));
        struct ARMCore* cpu = core->cpu;
        mc_orig = cpu->memory;
        cpu->memory.load32 = mc_load32;
        cpu->memory.load16 = mc_load16;
        cpu->memory.load8 = mc_load8;
        cpu->memory.store32 = mc_store32;
        cpu->memory.store16 = mc_store16;
        cpu->memory.store8 = mc_store8;
        cpu->memory.loadMultiple = mc_load_multiple;
        cpu->memory.storeMultiple = mc_store_multiple;
    }
    if (getenv("STACKCHECK")) {
        if (getenv("SYMS") && !nsyms)
            load_syms(getenv("SYMS"));
        struct ARMCore* cpu = core->cpu;
        sc_orig = cpu->memory;
        cpu->memory.store32 = sc_store32;
        cpu->memory.store16 = sc_store16;
        cpu->memory.store8 = sc_store8;
        cpu->memory.storeMultiple = sc_store_multiple;
    }
    int profile_n = getenv("PROFILE") ? atoi(getenv("PROFILE")) : 0;
    int profile_from = getenv("PROFILE_FROM") ? atoi(getenv("PROFILE_FROM")) : 1;
    unsigned long long profile_total = 0;
    if (profile_n && !nsyms)
        load_syms(getenv("SYMS"));
    if (getenv("PCS")) {
        pcs_lo = strtoul(getenv("PCS"), NULL, 16) & ~1u;
        const char* colon = strchr(getenv("PCS"), ':');
        pcs_hi = colon ? strtoul(colon + 1, NULL, 16) : pcs_lo;
        pcs_cycles = calloc((pcs_hi - pcs_lo) / 2 + 1, sizeof *pcs_cycles);
        pcs_count = calloc((pcs_hi - pcs_lo) / 2 + 1, sizeof *pcs_count);
    }
    if (getenv("FORK")) {
        const char* spec = getenv("FORK");
        fork_addr = strtoul(spec, NULL, 16);
        const char* colon = strchr(spec, ':');
        for (const char* p = colon ? colon + 1 : ""; *p && fork_n < kForkMaxValues;) {
            fork_values[fork_n++] = strtoul(p, NULL, 10);
            const char* comma = strchr(p, ',');
            p = comma ? comma + 1 : p + strlen(p);
        }
    }
    int frames = atoi(argv[2]);
    const char* shots = argc > 3 ? argv[3] : "";
    const char* prefix = argc > 4 ? argv[4] : "shot";
    for (int i = 1; i <= frames; ++i) {
        current_frame = i;
        // Frames counted from AFTER's line, when there is one.
        int t = !after_text ? i : after_frame ? i - after_frame : 0;
        if (after_text && !after_frame)
            core->setKeys(core, schedule_keys(&pre, i));
        else
            core->setKeys(core, schedule_keys(&keys, t));
        bool profiling = profile_n && t >= profile_from && t < profile_from + profile_n &&
                         (!after_text || after_frame);
        if (profiling)
            profile_total += profile_frame(core);
        else
            core->runFrame(core);
        if (getenv("STATE_OUT") && atoi(getenv("STATE_OUT")) == i) {
            struct VFile* vf = VFileOpen(strchr(getenv("STATE_OUT"), ':') + 1, O_CREAT | O_TRUNC | O_RDWR);
            if (!vf || !mCoreSaveStateNamed(core, vf, SAVESTATE_SAVEDATA | SAVESTATE_RTC))
                fprintf(stderr, "%s: can't save the state\n", argv[0]);
            if (vf)
                vf->close(vf);
        }
        if (audio_out)
            record_audio(core);
        if (profile_n && t == profile_from + profile_n - 1 && profile_total) {
            qsort(syms, nsyms, sizeof *syms, by_cycles);
            printf("profile: %llu cycles over %d frames\n", profile_total, profile_n);
            for (int k = 0; k < (getenv("PROFILE_TOP") ? atoi(getenv("PROFILE_TOP")) : 50) && syms[k].cycles; k++)
                printf("%5.1f%% %10llu  %s\n", 100.0 * syms[k].cycles / profile_total, syms[k].cycles,
                       syms[k].name);
            for (uint32_t a = pcs_lo; pcs_cycles && a < pcs_hi; a += 2) {
                if (pcs_cycles[(a - pcs_lo) / 2])
                    printf("pc %08x %10llu %8llu\n", a, pcs_cycles[(a - pcs_lo) / 2],
                           pcs_count[(a - pcs_lo) / 2]);
            }
            break;
        }
        const char* dump = getenv("DUMP");
        if (dump && t > 0 && atoi(dump) == t && strchr(dump, ':'))
            dump_memory(core, strchr(dump, ':') + 1);
        if (shot_frame == i) {
            char path[512];
            snprintf(path, sizeof path, "%s%s", prefix, shot_label);
            dump_memory(core, path);
            snprintf(path, sizeof path, "%s%s.png", prefix, shot_label);
            write_png(path, pixels, width, height, width);
            if (fork_n) {
                // (The first picture keeps the state: it is where the runs begin.)
                if (!fork_state) {
                    fork_state = VFileMemChunk(NULL, 0);
                    mCoreSaveStateNamed(core, fork_state, kForkSaveFlags);
                }
                if (!fork_run(core, i))
                    break;
            }
        }
        if (fork_run_from && i - fork_run_from > kForkMaxFrames) {
            printf("[%d] fork: no picture in %d frames\n", i, kForkMaxFrames);
            if (!fork_run(core, i))
                break;
        }
        if (stop_frame == i)
            break;
        for (const char* s = shots; *s && t > 0;) {
            if (atoi(s) == t) {
                char path[512];
                snprintf(path, sizeof path, "%s%d.png", prefix, t);
                write_png(path, pixels, width, height, width);
            }
            const char* comma = strchr(s, ',');
            s = comma ? comma + 1 : s + strlen(s);
        }
    }
    if (audio_out) {
        write_wav_header(audio_out, audio_bytes);
        fclose(audio_out);
    }
    core->deinit(core);
    return 0;
}
