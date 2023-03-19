#pragma once
#include "types.h"
#include <string.h>
#include <stdio.h>

extern uint8 g_ram[0x20000];
extern void RtlApuWrite(uint32 adr, uint8 val);
extern int snes_frame_counter;

extern uint8 *g_sram;
extern const uint8 *g_rom;
extern bool g_use_my_apu_code;
#define LONGPTR(t) {(t) & 0xffff, (t) >> 16}
extern bool g_debug_flag;

typedef void HandlerFunc(void);
typedef uint8 Func_V_A(void);

typedef void Func_V(void);

typedef CoroutineRet Func_V_Coroutine(void);
typedef uint8 Func_U8(void);
typedef uint16 Func_U16(void);
typedef void Func_Y_V(uint16 j);
typedef uint8 Func_Y_U8(uint16 j);
typedef void Func_X_V(uint16 j);
typedef uint16 Func_Y_Y(uint16 j);
typedef uint16 Func_Y();
typedef uint16 Func_XY_Y(uint16 k, uint16 j);


typedef void FuncXY_V(uint16 k, uint16 j);
typedef PairU16 Func_Y_To_PairU16(uint16 j);

const uint8 *RomPtr(uint32_t addr);
uint8 *IndirPtr(LongPtr ptr, uint16 offs);
uint8 IndirReadByte(LongPtr ptr, uint16 offs);
uint16 IndirReadWord(LongPtr ptr, uint16 offs);

void IndirWriteByte(LongPtr ptr, uint16 offs, uint8 value);
void IndirWriteWord(LongPtr ptr, uint16 offs, uint16 value);

static inline const uint8 *RomFixedPtr(uint32_t addr) { return &g_rom[(((addr >> 16) << 15) | (addr & 0x7fff)) & 0x3fffff]; }

struct LongPtr;
void mov24(LongPtr *dst, uint32 src);
void copy24(LongPtr *dst, LongPtr *src);
uint32 Load24(void *a);
void MemCpy(void *dst, const void *src, int size);
void Call(uint32 addr);
bool Unreachable();

#define INSTR_RETURN_ADDR(x) ((const uint16*)(uintptr_t)(x))
#define INSTR_INCR_BYTES(x, n) ((const uint16*)((uintptr_t)(x) + n))
#define INSTR_ADDR_TO_PTR(k, jp) ((uint8*)(jp) - (RomPtrWithBank(gEnemyData(k)->bank, 0x8000) - 0x8000))

#define INSTRB_RETURN_ADDR(x) ((const uint8*)(uintptr_t)(x))

#if defined(_DEBUG)
// Gives better warning messages but non inlined on tcc
static inline uint16 GET_WORD(const uint8 *p) { return *(uint16 *)(p); }
#else
#define GET_WORD(p) (*(uint16*)(p))
#endif

#define GET_BYTE(p) (*(uint8*)(p))

static inline uint8 *RomPtr_RAM(uint16_t addr) { assert(addr < 0x2000); return g_ram + addr; }
static inline const uint8 *RomPtr_80(uint16_t addr) { return RomPtr(0x800000 | addr); }
static inline const uint8 *RomPtr_81(uint16_t addr) { return RomPtr(0x810000 | addr); }
static inline const uint8 *RomPtr_82(uint16_t addr) { return RomPtr(0x820000 | addr); }
static inline const uint8 *RomPtr_83(uint16_t addr) { return RomPtr(0x830000 | addr); }
static inline const uint8 *RomPtr_84(uint16_t addr) { return RomPtr(0x840000 | addr); }
static inline const uint8 *RomPtr_85(uint16_t addr) { return RomPtr(0x850000 | addr); }
static inline const uint8 *RomPtr_86(uint16_t addr) { return RomPtr(0x860000 | addr); }
static inline const uint8 *RomPtr_87(uint16_t addr) { return RomPtr(0x870000 | addr); }
static inline const uint8 *RomPtr_88(uint16_t addr) { return RomPtr(0x880000 | addr); }
static inline const uint8 *RomPtr_89(uint16_t addr) { return RomPtr(0x890000 | addr); }
static inline const uint8 *RomPtr_8A(uint16_t addr) { return RomPtr(0x8a0000 | addr); }
static inline const uint8 *RomPtr_8B(uint16_t addr) { return RomPtr(0x8b0000 | addr); }
static inline const uint8 *RomPtr_8C(uint16_t addr) { return RomPtr(0x8c0000 | addr); }
static inline const uint8 *RomPtr_8D(uint16_t addr) { return RomPtr(0x8d0000 | addr); }
static inline const uint8 *RomPtr_8E(uint16_t addr) { return RomPtr(0x8e0000 | addr); }
static inline const uint8 *RomPtr_8F(uint16_t addr) { return RomPtr(0x8f0000 | addr); }
static inline const uint8 *RomPtr_90(uint16_t addr) { return RomPtr(0x900000 | addr); }
static inline const uint8 *RomPtr_91(uint16_t addr) { return RomPtr(0x910000 | addr); }
static inline const uint8 *RomPtr_92(uint16_t addr) { return RomPtr(0x920000 | addr); }
static inline const uint8 *RomPtr_93(uint16_t addr) { return RomPtr(0x930000 | addr); }
static inline const uint8 *RomPtr_94(uint16_t addr) { return RomPtr(0x940000 | addr); }
static inline const uint8 *RomPtr_95(uint16_t addr) { return RomPtr(0x950000 | addr); }
static inline const uint8 *RomPtr_96(uint16_t addr) { return RomPtr(0x960000 | addr); }
static inline const uint8 *RomPtr_97(uint16_t addr) { return RomPtr(0x970000 | addr); }
static inline const uint8 *RomPtr_98(uint16_t addr) { return RomPtr(0x980000 | addr); }
static inline const uint8 *RomPtr_99(uint16_t addr) { return RomPtr(0x990000 | addr); }
static inline const uint8 *RomPtr_9A(uint16_t addr) { return RomPtr(0x9a0000 | addr); }
static inline const uint8 *RomPtr_9B(uint16_t addr) { return RomPtr(0x9b0000 | addr); }
static inline const uint8 *RomPtr_9C(uint16_t addr) { return RomPtr(0x9c0000 | addr); }
static inline const uint8 *RomPtr_9D(uint16_t addr) { return RomPtr(0x9d0000 | addr); }
static inline const uint8 *RomPtr_9E(uint16_t addr) { return RomPtr(0x9e0000 | addr); }
static inline const uint8 *RomPtr_9F(uint16_t addr) { return RomPtr(0x9f0000 | addr); }
static inline const uint8 *RomPtr_A0(uint16_t addr) { return RomPtr(0xa00000 | addr); }
static inline const uint8 *RomPtr_A1(uint16_t addr) { return RomPtr(0xa10000 | addr); }
static inline const uint8 *RomPtr_A2(uint16_t addr) { return RomPtr(0xa20000 | addr); }
static inline const uint8 *RomPtr_A3(uint16_t addr) { return RomPtr(0xa30000 | addr); }
static inline const uint8 *RomPtr_A4(uint16_t addr) { return RomPtr(0xa40000 | addr); }
static inline const uint8 *RomPtr_A5(uint16_t addr) { return RomPtr(0xa50000 | addr); }
static inline const uint8 *RomPtr_A6(uint16_t addr) { return RomPtr(0xa60000 | addr); }
static inline const uint8 *RomPtr_A7(uint16_t addr) { return RomPtr(0xa70000 | addr); }
static inline const uint8 *RomPtr_A8(uint16_t addr) { return RomPtr(0xa80000 | addr); }
static inline const uint8 *RomPtr_A9(uint16_t addr) { return RomPtr(0xa90000 | addr); }
static inline const uint8 *RomPtr_AA(uint16_t addr) { return RomPtr(0xaa0000 | addr); }
static inline const uint8 *RomPtr_AB(uint16_t addr) { return RomPtr(0xab0000 | addr); }
static inline const uint8 *RomPtr_AC(uint16_t addr) { return RomPtr(0xac0000 | addr); }
static inline const uint8 *RomPtr_AD(uint16_t addr) { return RomPtr(0xad0000 | addr); }
static inline const uint8 *RomPtr_AE(uint16_t addr) { return RomPtr(0xae0000 | addr); }
static inline const uint8 *RomPtr_AF(uint16_t addr) { return RomPtr(0xaf0000 | addr); }
static inline const uint8 *RomPtr_B2(uint16_t addr) { return RomPtr(0xb20000 | addr); }
static inline const uint8 *RomPtr_B3(uint16_t addr) { return RomPtr(0xb30000 | addr); }
static inline const uint8 *RomPtr_B4(uint16_t addr) { return RomPtr(0xb40000 | addr); }
static inline const uint8 *RomPtr_B7(uint16_t addr) { return RomPtr(0xb70000 | addr); }
static inline const uint8 *RomPtrWithBank(uint8 bank, uint16_t addr) { return RomPtr((bank << 16) | addr); }

void WriteReg(uint16 reg, uint8 value);
void WriteRegWord(uint16 reg, uint16 value);
uint16 ReadRegWord(uint16 reg);
uint8 ReadReg(uint16 reg);

typedef void RunFrameFunc(uint16 input, int run_what);
typedef void SyncAllFunc();

void RtlReset(int mode);
void RtlSetupEmuCallbacks(uint8 *emu_ram, RunFrameFunc *func, SyncAllFunc *sync_all);
void RtlClearKeyLog();
void RtlStopReplay();

enum {
  kSaveLoad_Save = 1,
  kSaveLoad_Load = 2,
  kSaveLoad_Replay = 3,
};

void RtlSaveLoad(int cmd, int slot);
void RtlCheat(char c);
void RtlApuLock();
void RtlApuUnlock();
void RtlApuUpload(const uint8 *p);
void RtlRenderAudio(int16 *audio_buffer, int samples, int channels);
void RtlPushApuState();
bool RtlRunFrame(int inputs);
void RtlReadSram();
void RtlWriteSram();
void RtlSaveSnapshot(const char *filename, bool saving_with_bug);
void RtlUpdateSnesPatchForBugfix();

uint16 Mult8x8(uint8 a, uint8 b);
uint16 SnesDivide(uint16 a, uint8 b);
uint16 SnesModulus(uint16 a, uint8 b);

typedef struct SpcPlayer SpcPlayer;
extern SpcPlayer *g_spc_player;

enum {
  kJoypadL_A = 0x80,
  kJoypadL_X = 0x40,
  kJoypadL_L = 0x20,
  kJoypadL_R = 0x10,

  kJoypadH_B = 0x80,
  kJoypadH_Y = 0x40,
  kJoypadH_Select = 0x20,
  kJoypadH_Start = 0x10,

  kJoypadH_Up = 0x8,
  kJoypadH_Down = 0x4,
  kJoypadH_Left = 0x2,
  kJoypadH_Right = 0x1,

  kJoypadH_AnyDir = 0xf,
};

void Negate32(const uint16 *src_hi, const uint16 *src_lo, uint16 *dst_hi, uint16 *dst_lo);

struct OamEnt;
struct VramWriteEntry;


PairU16 MakePairU16(uint16 k, uint16 j);

#define kPoseParams ((SamusPoseParams*)RomFixedPtr(0x91b629))
#define kAtmosphericGraphicAnimationTimers ((uint16*)RomFixedPtr(0x908b93))
#define kAtmosphericTypeNumFrames ((uint16*)RomFixedPtr(0x908bef))
#define g_off_908BFF ((uint16*)RomFixedPtr(0x908bff))
#define g_stru_90A83A ((DisableMinimapAndMarkBossRoomAsExploredEnt*)RomFixedPtr(0x90a83a))
#define kPlayerPoseToPtr ((uint16*)RomFixedPtr(0x90c7df))
#define kDrawArmCannon_Tab2 ((uint16*)RomFixedPtr(0x90c7a5))
extern const int16 kSinCosTable8bit_Sext[320];
#define kPoseTransitionTable ((uint16*)RomFixedPtr(0x919ee2))
#define kDemoSetDefPtrs ((uint16*)RomFixedPtr(0x918885))
#define kSpeedBoostToCtr ((uint16*)RomFixedPtr(0x91b61f))
#define kSpeedBoostToAnimFramePtr ((uint16 *)RomFixedPtr(0x91B5DE))
#define kSamusPoseToBaseSpritemapIndexTop ((uint16*)RomFixedPtr(0x929263))
#define kSamusPoseToBaseSpritemapIndexBottom ((uint16*)RomFixedPtr(0x92945d))
#define kSamusAnimationDelayData ((uint16*)RomFixedPtr(0x91b010))
#define kCommonEnemySpeeds_Linear ((uint16*)RomFixedPtr(0xa28187))
#define kCommonEnemySpeeds_Quadratic ((uint16*)RomFixedPtr(0xa2838f))
#define kSine16bit ((uint16*)RomFixedPtr(0xa0b1c3))
#define kOamExtra_X8Small_And_Large ((uint16*)RomFixedPtr(0x81839f))
#define kOamExtra_Address_And_X8Large ((uint16*)RomFixedPtr(0x81859f))
#define kTanTable ((uint16*)RomFixedPtr(0x91c9d4))

void CallEnemyAi(uint32 ea);
void CallEnemyPreInstr(uint32 ea);
const uint16 *CallEnemyInstr(uint32 ea, uint16 k, const uint16 *jp);

void CalculateBlockContainingPixelPos(uint16 xpos, uint16 ypos);

/* 148 */
typedef enum SnesRegs {
  INIDISP = 0x2100,
  OBSEL = 0x2101,
  OAMADDL = 0x2102,
  OAMADDH = 0x2103,
  OAMDATA = 0x2104,
  BGMODE = 0x2105,
  MOSAIC = 0x2106,
  BG1SC = 0x2107,
  BG2SC = 0x2108,
  BG3SC = 0x2109,
  BG4SC = 0x210A,
  BG12NBA = 0x210B,
  BG34NBA = 0x210C,
  BG1HOFS = 0x210D,
  BG1VOFS = 0x210E,
  BG2HOFS = 0x210F,
  BG2VOFS = 0x2110,
  BG3HOFS = 0x2111,
  BG3VOFS = 0x2112,
  BG4HOFS = 0x2113,
  BG4VOFS = 0x2114,
  VMAIN = 0x2115,
  VMADDL = 0x2116,
  VMADDH = 0x2117,
  VMDATAL = 0x2118,
  VMDATAH = 0x2119,
  M7SEL = 0x211A,
  M7A = 0x211B,
  M7B = 0x211C,
  M7C = 0x211D,
  M7D = 0x211E,
  M7X = 0x211F,
  M7Y = 0x2120,
  CGADD = 0x2121,
  CGDATA = 0x2122,
  W12SEL = 0x2123,
  W34SEL = 0x2124,
  WOBJSEL = 0x2125,
  WH0 = 0x2126,
  WH1 = 0x2127,
  WH2 = 0x2128,
  WH3 = 0x2129,
  WBGLOG = 0x212A,
  WOBJLOG = 0x212B,
  TM = 0x212C,
  TS = 0x212D,
  TMW = 0x212E,
  TSW = 0x212F,
  CGWSEL = 0x2130,
  CGADSUB = 0x2131,
  COLDATA = 0x2132,
  SETINI = 0x2133,
  MPYL = 0x2134,
  MPYM = 0x2135,
  MPYH = 0x2136,
  SLHV = 0x2137,
  RDOAM = 0x2138,
  RDVRAML = 0x2139,
  RDVRAMH = 0x213A,
  RDCGRAM = 0x213B,
  OPHCT = 0x213C,
  OPVCT = 0x213D,
  STAT77 = 0x213E,
  STAT78 = 0x213F,
  APUI00 = 0x2140,
  APUI01 = 0x2141,
  APUI02 = 0x2142,
  APUI03 = 0x2143,
  WMDATA = 0x2180,
  WMADDL = 0x2181,
  WMADDM = 0x2182,
  WMADDH = 0x2183,
  JOYA = 0x4016,
  JOYB = 0x4017,
  NMITIMEN = 0x4200,
  WRIO = 0x4201,
  WRMPYA = 0x4202,
  WRMPYB = 0x4203,
  WRDIVL = 0x4204,
  WRDIVH = 0x4205,
  WRDIVB = 0x4206,
  HTIMEL = 0x4207,
  HTIMEH = 0x4208,
  VTIMEL = 0x4209,
  VTIMEH = 0x420A,
  MDMAEN = 0x420B,
  HDMAEN = 0x420C,
  MEMSEL = 0x420D,
  RDNMI = 0x4210,
  TIMEUP = 0x4211,
  HVBJOY = 0x4212,
  RDIO = 0x4213,
  RDDIVL = 0x4214,
  RDDIVH = 0x4215,
  RDMPYL = 0x4216,
  RDMPYH = 0x4217,
  JOY1L = 0x4218,
  JOY1H = 0x4219,
  JOY2L = 0x421A,
  JOY2H = 0x421B,
  JOY3L = 0x421C,
  JOY3H = 0x421D,
  JOY4L = 0x421E,
  JOY4H = 0x421F,
  DMAP0 = 0x4300,
  BBAD0 = 0x4301,
  A1T0L = 0x4302,
  A1T0H = 0x4303,
  A1B0 = 0x4304,
  DAS0L = 0x4305,
  DAS0H = 0x4306,
  DAS00 = 0x4307,
  A2A0L = 0x4308,
  A2A0H = 0x4309,
  NTRL0 = 0x430A,
  UNUSED0 = 0x430B,
  MIRR0 = 0x430F,
  DMAP1 = 0x4310,
  BBAD1 = 0x4311,
  A1T1L = 0x4312,
  A1T1H = 0x4313,
  A1B1 = 0x4314,
  DAS1L = 0x4315,
  DAS1H = 0x4316,
  DAS10 = 0x4317,
  A2A1L = 0x4318,
  A2A1H = 0x4319,
  NTRL1 = 0x431A,
  UNUSED1 = 0x431B,
  MIRR1 = 0x431F,
  DMAP2 = 0x4320,
  BBAD2 = 0x4321,
  A1T2L = 0x4322,
  A1T2H = 0x4323,
  A1B2 = 0x4324,
  DAS2L = 0x4325,
  DAS2H = 0x4326,
  DAS20 = 0x4327,
  A2A2L = 0x4328,
  A2A2H = 0x4329,
  NTRL2 = 0x432A,
  UNUSED2 = 0x432B,
  MIRR2 = 0x432F,
  DMAP3 = 0x4330,
  BBAD3 = 0x4331,
  A1T3L = 0x4332,
  A1T3H = 0x4333,
  A1B3 = 0x4334,
  DAS3L = 0x4335,
  DAS3H = 0x4336,
  DAS30 = 0x4337,
  A2A3L = 0x4338,
  A2A3H = 0x4339,
  NTRL3 = 0x433A,
  UNUSED3 = 0x433B,
  MIRR3 = 0x433F,
  DMAP4 = 0x4340,
  BBAD4 = 0x4341,
  A1T4L = 0x4342,
  A1T4H = 0x4343,
  A1B4 = 0x4344,
  DAS4L = 0x4345,
  DAS4H = 0x4346,
  DAS40 = 0x4347,
  A2A4L = 0x4348,
  A2A4H = 0x4349,
  NTRL4 = 0x434A,
  UNUSED4 = 0x434B,
  MIRR4 = 0x434F,
  DMAP5 = 0x4350,
  BBAD5 = 0x4351,
  A1T5L = 0x4352,
  A1T5H = 0x4353,
  A1B5 = 0x4354,
  DAS5L = 0x4355,
  DAS5H = 0x4356,
  DAS50 = 0x4357,
  A2A5L = 0x4358,
  A2A5H = 0x4359,
  NTRL5 = 0x435A,
  UNUSED5 = 0x435B,
  MIRR5 = 0x435F,
  DMAP6 = 0x4360,
  BBAD6 = 0x4361,
  A1T6L = 0x4362,
  A1T6H = 0x4363,
  A1B6 = 0x4364,
  DAS6L = 0x4365,
  DAS6H = 0x4366,
  DAS60 = 0x4367,
  A2A6L = 0x4368,
  A2A6H = 0x4369,
  NTRL6 = 0x436A,
  UNUSED6 = 0x436B,
  MIRR6 = 0x436F,
  DMAP7 = 0x4370,
  BBAD7 = 0x4371,
  A1T7L = 0x4372,
  A1T7H = 0x4373,
  A1B7 = 0x4374,
  DAS7L = 0x4375,
  DAS7H = 0x4376,
  DAS70 = 0x4377,
  A2A7L = 0x4378,
  A2A7H = 0x4379,
  NTRL7 = 0x437A,
  UNUSED7 = 0x437B,
  MIRR7 = 0x437F,
} SnesRegs;
