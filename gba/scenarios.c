// For testing (-DSM_SCENARIOS=ON, tools/scenarios.sh): plays a list of
// scenarios one after another without a player, to check each one's
// picture, speed and sound. Each spawns Samus with the developer tools
// (dev_tools.c: a load station, or a boss's door, with a loadout and a set
// of beams), then presses buttons by a script, frame by frame, printing
// "tour: shot NAME-N" at the script's shots, for smrun's SHOT_ON (as
// tour.c's do), and "scenario: NAME" as each starts. The game's own
// "sm: 60 frames in N vblanks" lines give the speed (not counting a shot's
// hold). After a script the game is reset, back to the title, for the
// next scenario.
//
// Which scenarios run is read from the buttons smrun holds as the cartridge
// starts, KEYS=1:100:FIRST,COUNT (as of the game's frames 20 and 130): the ten
// buttons the bits of a number (A is 1, B 2, Select 4, and so on to L 512):
// the scenario to start with, and how many (0: to the list's end), with
// COUNT's bit 9 (L) for no holds on shots (to record the sound). With
// -DSM_SCENARIO_ONLY=name1,name2 the list is only those, by name (a
// trailing * for a prefix).
//
// A script is words, separated by spaces:
//   BUTTONS*N  holds the buttons, joined by '+' (A B SELECT START RIGHT LEFT
//              UP DOWN R L, or '-' for none), for N frames
//   @          a shot of the frame the next word begins
//   ( ... )xN  the words between, N times
//   ( ... )WOFF,N  the words between, again and again until the 16-bit word at
//              OFF (hex, in the game's RAM: g_ram + OFF) is at least N (decimal),
//              or ( ... )VOFF,N until it is at most N: for a fight, played until
//              the boss's health, place or state says it has gone on (the
//              first enemy's x position is f7a, its health f8c, its variable A
//              fa8; Samus's x position af6; the boss bits of Norfair d82a). At
//              most 1500 times round, for a fight that never gets there.
//   `N         Samus keeps N pixels to the left of the first enemy (at most 12
//              off: put there, a player's footwork, for a fight where he must stay
//              in reach), until `0
//   >X,Y       Samus to that place (pixels)
//   <DX,DY     Samus DX pixels from the first enemy (on the room's centre side of it) and
//              DY below its centre (-DY above), turned toward it: for a boss that moves
//   #HHHH      Samus through the door with that definition (hex, in bank $83) as
//              the room tour does (tour.c): into the room it leads to, at its
//              other side, and the script goes on once she has her controls
//              (on the screen: if the door leaves her off it, she is put
//              where the camera has the door's far side)
//   P          no power bombs, for the next room to load without them: Crateria's
//              landing site has its rain until Samus has them
//   &N         the area's boss bits to N, for the rooms Samus goes on to: 1 is
//              the Wrecked Ship's power on (Phantoon dead), and 4 Maridia's
//              Draygon dead
//   $N         the timer's status N (1 starts Ceres's escape, 2 Mother
//              Brain's, with the timer's tiles, which her death loads), and
//              the timer's handling
//   =N         the game state N (32: Ceres's elevator ride, then its explosion
//              and the blackout: Mode 7 scenes)
//   ^N         the first enemy's (the boss's) health to N, to reach a late phase of a fight
//              that a script can't aim its way to (it can't see the boss)
//   _N         Samus's energy to N, with no reserve and no refills (Ceres's Ridley leaves when it
//              is under 30), and + gives her them back
//   %N         the message box N, as an item's pickup shows it (5 is the missile
//              tanks', 28 the save station's question)
//   H, h       Samus's energy is no longer refilled every frame (H), or is (h), the
//              ammunition still is (unlike _N): for a drain that the game goes on
//              by (Mother Brain's rainbow beam)
//   :ADDR=N    the game's 16-bit word at ADDR (hex) to N (decimal)
//   GN         Mother Brain's tank takes N more hits (see kMbFull)
//   !          Samus dies (no energy, reserve or refills): the game over
//   ~N         the lava, acid or water's surface to N pixels below Samus's
//              centre (-N for above), at its base: for a room's lava where she
//              can't go (the camera follows her only so far)
// The buttons are the GBA's (input.c makes the game's of them): B shoots, R
// readies missiles (with Select, the others), L aims. Samus can't die (but
// for the !), and never runs out of ammunition.
#include "scenarios.h"

#include <stdio.h>
#include <string.h>

#include "dev_tools.h"
#include "funcs.h"
#include "gba_hw.h"
#include "ida_types.h"
#include "input.h"
#include "sm_rtl.h"
#include "variables.h"

typedef struct Scenario {
  const char *name;
  // The load station of the area (0 Crateria, 1 Brinstar, 2 Norfair, 3
  // Wrecked Ship, 4 Maridia, 5 Tourian), as the map numbers them (16 on
  // are the debug stations); or, with |boss| from 1, the area's boss door
  // of that number (dev_tools.c's, in the map's order).
  uint8_t area, station, boss;
  // The loadout is ALL ITEMS; with these beams (the game's bits) instead, if
  // not -1.
  int16_t beams;
  const char *script;
} Scenario;

// The beam sets (wave 1, ice 2, spazer 4, plasma 8) in the game's bits,
// without the charge beam (0x1000) and with.
#define BEAMS(n) (n)
#define CHARGED(n) (0x1000 + (n))

// ---- The weapons, in the room east of the landing site (Crateria's debug
// station 17, 94FD): Samus stands on a ledge, 90 pixels from a door that she
// faces, with a block ramp under her and water below it. Shots hit the door
// and the blocks.
#define kRoom 0, 17, 0
// Facing the door (a tap on Left turns her), shots in the air and at the door,
// the aimed ones, and a charged one: held, let go, in flight, at the door.
#define kBeamScript \
  "-*20 LEFT*4 -*30 @ B*2 -*4 @ -*4 @ -*12 @ (B*2 -*8)x3 @ -*40 @ " \
  "L+DOWN+B*3 -*5 @ -*10 @ UP+B*3 -*5 @ -*20 @ " \
  "B*130 @ -*1 -*5 @ -*8 @ -*30 @"

#define BEAM(n, name) \
  {"beam-" name, kRoom, BEAMS(n), kBeamScript}, \
  {"beam-" name "-charge", kRoom, CHARGED(n), kBeamScript}

// Samus faces the door; R readies missiles (then, with Select, the next of
// supers, power bombs, the grapple beam and the X-ray scope).
#define kFace "-*20 LEFT*4 -*30 @ "
#define kSelectSupers "R*2 R+SELECT*2 R*2 "
#define kSelectPowerBombs kSelectSupers kSelectSupers
#define kSelectGrapple kSelectPowerBombs kSelectSupers
#define kSelectXray kSelectGrapple kSelectSupers
// Down twice: crouch, then the morph ball.
#define kMorph "DOWN*10 -*6 DOWN*10 -*20 "

// ---- The bosses: each with the ALL ITEMS loadout, and a script that fights
// for 70 seconds, a shot every 10: shots, jumps, charged shots, missiles, and
// then super missiles.
#define kFight \
  "(RIGHT*30 B*3 -*5 A+RIGHT*25 L+B*3 -*10 LEFT*30 B*100 -*2 R+B*3 -*8 " \
  "A+LEFT*25 B*3 -*5 UP+B*3 -*10)x2"
#define kBossScript \
  "-*30 @ (" kFight " -*70 @)x3 " kSelectSupers "(" kFight " -*70 @)x3"

// Phantoon: his eye opens to be hit, in the pattern's turn and while his fireballs rain: a
// missile does 100 (three in an eye's time are a swoop), a super missile 300 (one is his rage,
// and then he comes back to be hit again). So: missiles and then supers, from the floor at
// five places along it, each aimed up the diagonal and straight up, a shot of the frame in ten.
// (^300: his last hit, once the rage is over.) Then out of the room by its door, to the next.
#define PH_FIRE(aim) "@ " aim "*3 -*7 "
#define PH_BLOCK(x) \
  ">" #x ",187 (" PH_FIRE("L+R+B") PH_FIRE("UP+R+B") PH_FIRE("L+R+B") PH_FIRE("UP+R+B") ")x3 "
#define PH_ROUND \
  PH_BLOCK(38) PH_BLOCK(78) PH_BLOCK(118) PH_BLOCK(158) PH_BLOCK(198)
// A shot every 20 frames, as the room fades in and Phantoon appears, and in the next room.
#define PH_IDLE "(@ -*19)x30 "
#define kPhantoonFull \
  PH_IDLE "(" PH_ROUND ")x4 " kSelectSupers "(" PH_ROUND ")x2 ^300 (" PH_ROUND ")x7 #a2c4 " PH_IDLE

// Draygon: his goop (from the wall turrets) slows Samus, and then he grabs her and swirls her
// round the room. The grapple beam fired as he carries her, held while it connects with a turret's
// block on the wall, shocks him: 256 a 8 frames. So: the fight's usual shots and jumps while he
// comes (his first goop and grab), then (^256, so one connection of eight frames does it, for
// how long the swirl takes to bring her to the wall) the beam, again and again for 14 frames in
// 16, in her facing direction, until he is dead (his death, the boss bit) and then out of the
// room by its door to the next, and a walk there.
#define kDraygonFull \
  "(@ RIGHT*30 @ B*3 -*5 @ A+RIGHT*25 L+B*3 -*10 @ LEFT*30 @ B*30 -*2 @ A+LEFT*25 B*3 -*5 " \
  "UP+B*3 -*10)x6 " kSelectGrapple "^256 (@ R+B*14 -*2)x300 #a978 " PH_IDLE "RIGHT*100 LEFT*100 "

#define BOSS(name, area, n) {"boss-" name, area, 0, n, -1, kBossScript}

// Kraid, from his rise to the item room after (the whole fight, 130 seconds):
// super missiles, one every 4 frames, at a jump's height. His mouth is the
// only place that hurts him, and only open: 300 a hit (1000 health). The
// first phase from the ledge by the door (a hit, and he grows and walks), the
// second from the platform (put there each time, as he knocks Samus off it):
// two hits and a last, then his death, sinking, the right door (shot open),
// the item room beyond (Samus put by the orb) and its door back.
#define kKraidJump "(@ A+R+B*2 A+R*2)x12 -*16"
#define kKraidFull \
  "-*30 @ " kSelectSupers "RIGHT*20 (@ -*15)x70 (" kKraidJump ")x10 " \
  "(>248,250 -*8 " kKraidJump ")x24 (@ -*15)x80 (@ RIGHT*15)x60 (@ A+RIGHT*15)x40 " \
  "(@ RIGHT*15)x20 B*3 R+B*3 (@ RIGHT*15)x8 (@ -*15)x16 >120,158 (@ -*15)x40 A*3 " \
  "(@ -*15)x30 (@ LEFT*15)x20 B*3 (@ LEFT*15)x40 (@ -*15)x30"

// ---- Ridley's fight, with a shot every 4 frames: supers (she has plenty) up, ahead, and
// jumping each way. A script can't see Ridley, so ^ sets his health to reach each phase
// (his flight changes at 14400 and 9000, his colour at 9000, 5400 and 1800) after the
// first one's honest hits.
#define kSuperShots(keys) "(@ " keys "+R+B*3 -*1 @ -*4)x2 "
#define kRidleyCycle \
  "(" kSuperShots("-") kSuperShots("UP") kSuperShots("A+RIGHT") kSuperShots("A+LEFT") ")"
#define kRidleyFull \
  "-*30 @ " kSelectSupers kRidleyCycle "x20 ^14000 " kRidleyCycle "x15 ^8900 " kRidleyCycle \
  "x15 ^5300 " kRidleyCycle "x15 ^1700 " kRidleyCycle "x15"
// The kill: shots at 1 health until one hits, the death with a shot every 14 frames, and then
// the door out (to the room with the energy tank).
#define kRidleyDeath \
  "-*30 @ " kSelectSupers kRidleyCycle "x4 ^1500 " kRidleyCycle "x6 ^1 " kRidleyCycle "x30 " \
  "(@ -*14)x200 #98b2 -*60 @ (RIGHT*40 @ -*10 @ LEFT*40 @ -*10)x3 -*60 @"

// ---- Ceres's Ridley: the room's wait for him, the fight (his hits and flights: Samus shoots
// him, but he can't be hurt), his leaving with the baby once Samus is under 30 energy (_29), the
// escape's start (the timer, the shaking), the run back through the rooms (a door each, and a
// walk) and the arrival at the elevator, which the explosion follows (=32).
#define kCeresRun(door) "#" door " (@ -*8)x20 LEFT*60 (@ -*8)x20 "
#define kCeresRidley \
  "#abac -*30 @ (-*20 @)x30 (@ B*3 -*1 @ -*4)x200 _29 (@ -*4)x200 + " \
  kCeresRun("abb8") kCeresRun("aba0") kCeresRun("ab88") kCeresRun("ab70") kCeresRun("ab58") "(@ -*20)x45 " \
  "=32 (@ -*10)x200"

// Whole fights, with a shot every 4 frames (to see a layer or a sprite go for a
// few frames): the buttons held for N groups of 4 frames, each starting with a shot.
#define D(buttons, n) "(@ " buttons "*4)x" #n " "
// A walk and a jump each way, a missile after each, readied once before.
#define kMissileCycle \
  D("RIGHT", 5) D("R+B", 1) D("-", 2) D("A+RIGHT", 6) D("R+B", 1) D("-", 2) \
  D("LEFT", 5) D("R+B", 1) D("-", 2) D("A+LEFT", 6) D("R+B", 1) D("-", 2)

// Botwoon (enemy 0 is his head, 3000 health, in and out of the wall's holes): Samus stands
// beside it with missiles, and he dies after about 120 of these (nearly 70 s), exploding
// in pieces; then the room, and a walk right and left.
#define kBotwoonFull \
  "-*30 @ R*2 (@ <40,5 R+B*2 @ -*4 (@ -*4)x6)x130 " D("-", 100) D("RIGHT", 60) D("LEFT", 60) D("-", 20)

// Bomb Torizo wakes at about the 5th cycle, and dies in the 15th; then his
// death, then a jump up to the door he came through and out of the room.
#define kBombTorizoFull \
  "-*30 @ R*2 (" kMissileCycle ")x16 " D("-", 100) D("LEFT", 25) D("A+LEFT", 2) D("A+LEFT", 2) \
  D("LEFT", 5) D("R+B", 1) D("LEFT", 8) D("-", 60)

// Spore Spawn's core is exposed only while he waits, open, at the end of a swing, and only
// to a shot level with it (his plant covers the rest): Samus is put beside it each time, with
// super missiles. He dies after four hits (palette steps at 770, 410 and 70 health).
#define kSporeSpawnFull \
  "-*30 @ >128,715 -*10 " kSelectSupers "(@ <40,5 R+B*2 @ -*4 @ -*2)x250 " D("-", 120)


// The lava (or acid) first at Samus's feet and then rising past her, from a
// station of a room that has it.
#define kLavaScript "-*30 @ ~40 -*60 @ ~20 -*60 @ ~0 -*60 @ ~-30 -*60 @"

// ---- Mother Brain, from her door to her death (the :, G and ( )W/V words wait for
// the game's own state, as a player waits for the fight; see the top). Her
// fields in RAM (hex addresses): the tank's glass takes hits (PLM room
// argument 39, 1e15) to break at 18; then her brain (enemy 1) has health to lose
// (fcc), then her body (enemy 0) is in phase 1, 2 (standing, 7800), 3 (the baby
// Metroid's rescue) and 4 (the hyper beam). Her routine (fa8) goes up the
// code as the rainbow beam goes: 47477 (b975) starts to fire, 47710 (ba5e)
// is its end, as Samus's health runs out.
// Samus's missiles reach the brain from the air at the pillar's height (the places in the script);
// from the 8th hit on they explode on the glass instead, and the count stays there, so G makes
// the hits the game's own scene would take longer to get (a player's way round isn't known).
#define kMbHits "(LEFT*20 R+L+B*2 -*10 RIGHT*20 R+UP+B*2 -*10 R+B*2 -*20 @)x6"
#define kMbFull \
  "-*60 RIGHT*30 @ (-*9 @)x6 " \
  "(>56,92 R+B*2 -*4 G1 @ -*9)W1e15,18 " \
  "(>94,106 R+B*2 -*6 @ -*5)Vfcc,0 (-*16 @)W7800,2 -*100 " \
  kMbHits " :fcc=4400 " kMbHits " " \
  "H :fcc=0 :9c2=600 (-*20 @)Wfa8,47477 (-*6 @)Wfa8,47710 " \
  "(-*20 @)W7800,3 (-*30 @)W7800,4 h -*60 (@ -*10)x6 " \
  "(LEFT*10 B*30 UP+B*20 -*4 @)x5 :fcc=3000 (LEFT*10 B*30 UP+B*20 -*4 @)Vfcc,0 " \
  "(-*15 @)W943,1 -*100 @"

// ---- The escape from Zebes: the timer running (event 14, the time bomb, in word d820 with
// the events 0 and 2 that Mother Brain's fight sets), from Tourian's station 16 through Mother
// Brain's room's right door and the escape rooms to Crateria, and the landing site's ship, where
// Down takes Samus aboard (the ship's code wants her under it, and the event): the ship's
// lift-off, then the ending and the credits, which go on by themselves.
#define kEscRoom "-*30 @ (A+RIGHT*30 @ RIGHT*40 @ B*3 -*10 @ A+LEFT*30 @ LEFT*40 @ -*10 @)x2"
#define kEscape \
  "-*30 &2 :d820=16389 $2 -*90 @ (-*30 @)x3 " \
  "#aa8c " kEscRoom " #aaec " kEscRoom " #ab04 " kEscRoom " #ab1c " kEscRoom " " \
  "#ab34 " kEscRoom " #8b3e " kEscRoom " #896a -*30 @ RIGHT*100 -*40 @ >1152,1088 -*60 @ DOWN*2 " \
  "(-*40 @)x150"

// ---- Tourian's Metroid rooms (each by its door): Samus walks into the room, shoots the first
// Metroid on its way to her, as ice freezes it, and walks under it, shooting missiles up (it takes
// five); then the others latch on, and she rolls into a ball and lays bombs, which shake them off.
#define kMetroids \
  "-*30 @ LEFT*60 (B*3 -*4 @)x25 LEFT*70 (R+UP+B*3 -*6 @)x16 (-*10 @)x4 " \
  "LEFT*200 -*10 @ (-*10 @)x16 DOWN*10 -*6 DOWN*10 -*20 @ (-*6 @)x6 R*10 R+B*2 (-*10 @)x12 " \
  "(B*2 -*14 @)x30 (-*30 @)x4"

// ---- Crateria and Brinstar: rooms entered by door (the # word: each door's
// definition is the room tour's, tour.c), a walk or a fight in each.
#define kWalk "-*30 @ RIGHT*100 @ LEFT*200 @ RIGHT*100 A+RIGHT*40 @ -*10 @"
// A fight: beams, jumps, charged shots and missiles, then supers, then (in the morph ball)
// bombs and a power bomb.
#define kBeamsAndSupers \
  "-*30 @ (RIGHT*40 B*3 -*4 A+RIGHT*20 L+B*3 -*8 LEFT*40 B*60 -*2 R+B*3 -*8 A+LEFT*20 " \
  "UP+B*3 -*8)x3 @ " kSelectSupers "(RIGHT*40 R+B*3 -*8 LEFT*40 R+B*3 -*8)x3 @ " kMorph
#define kBombs "(RIGHT*10 B*2 -*12 LEFT*10 B*2 -*12)x4 @ "
#define kRoomFight kBeamsAndSupers kBombs "R*10 R+B*2 -*60 @"
// The same, with a shot as each stage of the power bomb's blast goes by.
#define kPowerBombBlast \
  kBeamsAndSupers "@ " kBombs "R*10 @ R+B*2 -*10 @ -*10 @ -*10 @ -*10 @ -*10 @ -*20 @ -*30 @ -*30 @"
#define CB_FIGHT(room, door) {"cb-fight-" room, 0, 0, 0, -1, "#" door " " kRoomFight}

// A player's minute in a room: walks, jumps and shots each way, a charged
// shot, missiles and supers, then the morph ball with bombs and a power bomb.
#define kPlay \
  "-*30 @ (RIGHT+B*40 -*5 A+RIGHT+B*30 L+B*3 LEFT+B*40 A+LEFT*30 -*5)x2 @ " \
  "RIGHT*80 B*100 -*2 @ LEFT*80 R+B*3 -*10 @ " kSelectSupers "R+B*3 -*10 @ " \
  "RIGHT*60 @ " kMorph "LEFT*40 B*2 -*20 B*2 -*20 B*2 -*30 @ " \
  "R*2 R+SELECT*2 R*2 R+SELECT*2 R*2 R+SELECT*2 R*2 -*10 R+B*2 -*40 @ " \
  "RIGHT*80 B*2 -*40 @ -*30 @"

// Each room of Norfair, from the first station through a door into it: a
// player's walk each way, shots, a charged shot and a missile.
#define NFR(room, door) \
  {"nfr-" #room, 2, 0, 0, -1, "-*20 #" #door " -*30 @ " kRoomWalk}
#define kRoomWalk \
  "(RIGHT+B*50 A+RIGHT+B*30 LEFT+B*50 A+LEFT*30)x2 @ RIGHT*40 B*80 -*2 @ " \
  "LEFT*40 R+B*3 -*20 @ -*30 @"

// ---- The Wrecked Ship and Maridia: rooms are entered by a door (the # word) from the
// area's debug station 16 (the ship's power is on with "&1" first). A room entered from the side
// is walked through, one from above or below climbed.
#define SHIP_OFF(name, door, script) {name, 3, 16, 0, -1, "-*10 #" door " " script}
#define SHIP_ON(name, door, script) {name, 3, 16, 0, -1, "-*10 &1 #" door " " script}
#define MARIDIA(name, door, script) {name, 4, 16, 0, -1, "-*10 #" door " " script}
// Walks in one direction (F) with a shot every so often, jumps, aimed shots, missiles and a
// charged shot; then morphs, lays bombs and unmorphs.
#define kWalkDir(F) \
  "-*20 @ (" F "+B*3 " F "*37 A+" F "*30 L+B*3 -*5 " F "*20 R+B*3 -*10 B*70 -*3 " F "*30 @)x3 " \
  "DOWN*10 -*6 DOWN*10 -*10 (B*2 -*20 " F "*20)x4 @ UP*10 -*60 @"
// Jumps, up and to both sides, shooting up and charging.
#define kClimb "-*20 @ (A*60 -*20 UP+B*3 -*10 A+RIGHT*40 A+LEFT*40 B*70 -*3 @)x4 -*60 @"
// Rolled into a ball, a power bomb every 3 seconds (with R, as only they can be readied).
#define kPowerBombs "DOWN*10 -*6 DOWN*10 -*10 (R+B*2 -*200 @)x8"

// ---- Tourian's rooms and Ceres's, entered by their doors from Tourian's
// debug station 16 (the doors are the room tour's: "tour: to ROOM by door
// HHHH"): Samus waits, walks and jumps both ways, shooting.
#define kRoomKeys \
  "-*90 @ (RIGHT*50 B*3 A+RIGHT*20 -*20)x3 @ (LEFT*50 B*3 A+LEFT*20 -*20)x3 @ " \
  "L+B*3 -*30 UP+B*3 -*60 @"
#define ROOM(name, door) {"room-" name, 5, 16, 0, -1, "#" door " " kRoomKeys}

// Pause, the map's scrolling, the equipment screen (R, which takes a second or two, and L
// back) and the unpause.
#define kPauseScript \
  "-*60 @ START*3 -*100 @ (RIGHT*3 -*10)x3 @ R*10 -*200 @ (DOWN*3 -*10)x6 @ L*10 -*200 @ " \
  "(LEFT*3 -*10)x3 @ START*3 -*100 @ -*60 @"

static const Scenario kScenarios[] = {
  BEAM(0, "power"),
  BEAM(1, "wave"),
  BEAM(2, "ice"),
  BEAM(3, "ice-wave"),
  BEAM(4, "spazer"),
  BEAM(5, "spazer-wave"),
  BEAM(6, "spazer-ice"),
  BEAM(7, "spazer-ice-wave"),
  BEAM(8, "plasma"),
  BEAM(9, "plasma-wave"),
  BEAM(10, "plasma-ice"),
  BEAM(11, "plasma-ice-wave"),
  {"missile", kRoom, BEAMS(0),
   kFace "R*10 @ R+B*2 -*5 @ -*10 @ (R+B*2 -*25)x3 @ -*40 @"},
  {"super", kRoom, BEAMS(0),
   kFace kSelectSupers "R*10 @ R+B*2 -*5 @ -*10 @ (R+B*2 -*25)x3 @ -*40 @"},
  {"bomb", kRoom, BEAMS(0),
   kFace kMorph "@ B*2 -*30 @ -*8 @ -*8 @ (B*2 -*12)x3 -*20 @ -*10 @ -*60 @"},
  // The charge beam's special attacks: power bombs readied, B held until the
  // beam is fully charged (120 frames), then let go.
#define COMBO(n, name) \
  {"combo-" name, kRoom, CHARGED(n), \
   kFace kSelectPowerBombs "R*10 @ R+B*60 @ R+B*58 @ R+B*6 @ R+B*20 @ -*10 @ -*30 @ -*60 @"}
  COMBO(1, "wave"),
  COMBO(2, "ice"),
  COMBO(4, "spazer"),
  COMBO(8, "plasma"),
  {"power-bomb", kRoom, BEAMS(0),
   kFace kMorph "R*10 @ R+B*2 -*10 @ -*20 @ -*30 @ -*40 @ -*60 @"},
  {"xray", kRoom, BEAMS(0),
   kFace kSelectXray "R*30 @ R*60 @ -*30 @"},
  {"grapple", 4, 17, 0, BEAMS(0),
   "-*20 >136,140 -*200 LEFT*4 -*60 @ " kSelectGrapple "R+UP+B*20 @ R+UP+B*40 @ R+UP+B*40 @ -*30 @"},
  {"screw-attack", kRoom, BEAMS(0),
   kFace "A+LEFT*30 @ A*30 @ A*30 @ -*60 @"},
  {"space-jump", kRoom, BEAMS(0),
   kFace "A+LEFT*30 -*4 (A*4 -*6)x12 @ A+RIGHT*30 @ -*60 @"},
  // Running right from the Wrecked Ship's debug station 16 (CA08), on 60
  // blocks of flat floor: the speed booster builds up in about 3 seconds.
  // Then a shinespark: crouch at speed, jump, aim.
  {"speed-booster", 3, 16, 0, BEAMS(0),
   "-*30 @ RIGHT*60 @ RIGHT*60 @ RIGHT*60 @ RIGHT*60 @ RIGHT*40 @ -*30 @"},
  {"shinespark", 3, 16, 0, BEAMS(0),
   "-*30 RIGHT*140 @ DOWN*4 -*20 @ A*4 -*4 RIGHT*3 -*10 @ -*20 @ -*20 @ -*40 @ -*60 @"},
  // ---- Norfair's lava and acid, in the rooms of its debug stations.
  {"lava-aab5", 2, 0, 0, -1, kLavaScript},
  {"lava-b0dd", 2, 1, 0, -1, kLavaScript},
  {"lava-b741", 2, 5, 0, -1, kLavaScript},
  {"lava-a7de", 2, 8, 0, -1, kLavaScript},
  {"lava-af3f", 2, 9, 0, -1, kLavaScript},
  {"lava-b236", 2, 10, 0, -1, kLavaScript},
  {"lava-a923", 2, 17, 0, -1, kLavaScript},
  {"lava-b37a", 2, 18, 0, -1, kLavaScript},
  {"lava-b283", 2, 21, 0, -1, kLavaScript},
  {"lava-b283-2", 2, 22, 0, -1, kLavaScript},
  // The landing site in rain (it rains until Samus has power bombs: the P), from its door at
  // the west, running east out of the room, and back.
  {"cb-rain", 0, 0, 0, -1,
   "P #896a -*30 @ RIGHT*150 @ RIGHT*150 @ RIGHT*150 @ RIGHT*150 @ RIGHT*150 @ RIGHT*150 @ "
   "LEFT*400 @ -*50 @"},
  // At the ship, as the debug station leaves her: along the landing site's floor.
  {"cb-landing", 0, 16, 0, -1,
   "-*30 @ RIGHT*100 @ RIGHT*100 @ RIGHT*100 @ LEFT*300 @ LEFT*100 @ -*10 @"},
  // Bomb Torizo's room (his, as a door from the room west of it leaves it).
  {"cb-torizo-room", 0, 0, 0, -1, "#8bc2 " kWalk},
  // An item's message box (Brinstar's super missile): the game waits, the picture is the
  // cartridge's alone.
  {"cb-item-box", 0, 0, 0, -1, "#8f3a " kWalk},
  // A power bomb's blast in a room of water (Brinstar's a0d2) and of fog (Crateria's 96ba).
  {"cb-pb-water", 0, 0, 0, -1, "#8e0e " kPowerBombBlast},
  {"cb-pb-fog", 0, 0, 0, -1, "#898e " kPowerBombBlast},
  CB_FIGHT("9ad9", "8c0a"),
  CB_FIGHT("9d19", "8e7a"),
  CB_FIGHT("9cb3", "8ce2"),
  CB_FIGHT("99f9", "8b4a"),
  CB_FIGHT("96ba", "898e"),
  CB_FIGHT("93fe", "8aea"),
  CB_FIGHT("92fd", "8916"),
  CB_FIGHT("99bd", "8be6"),
  CB_FIGHT("a0d2", "8e0e"),
  CB_FIGHT("9e52", "8e9e"),
  CB_FIGHT("9e9f", "8b9e"),
  CB_FIGHT("a322", "8b02"),
  CB_FIGHT("a408", "9102"),
  CB_FIGHT("a253", "907e"),
  CB_FIGHT("9b5b", "8f76"),
  CB_FIGHT("9fba", "8e92"),
  // ---- Norfair played the way a player would, from each station of its
  // rooms (the map's numbers: 0 to 7 saves, 8 to 15 elevators, 16 on debug), and then each
  // room by its door.
  {"nf-0-aab5", 2, 0, 0, -1, kPlay},
  {"nf-1-b0dd", 2, 1, 0, -1, kPlay},
  {"nf-2-b167", 2, 2, 0, -1, kPlay},
  {"nf-3-b192", 2, 3, 0, -1, kPlay},
  {"nf-4-b1bb", 2, 4, 0, -1, kPlay},
  {"nf-5-b741", 2, 5, 0, -1, kPlay},
  {"nf-8-a7de", 2, 8, 0, -1, kPlay},
  {"nf-9-af3f", 2, 9, 0, -1, kPlay},
  {"nf-10-b236", 2, 10, 0, -1, kPlay},
  {"nf-17-a923", 2, 17, 0, -1, kPlay},
  {"nf-18-b37a", 2, 18, 0, -1, kPlay},
  {"nf-19-aa82", 2, 19, 0, -1, kPlay},
  {"nf-20-b236", 2, 20, 0, -1, kPlay},
  {"nf-22-b283", 2, 22, 0, -1, kPlay},
  NFR(a75d, 931e),
  NFR(a788, 9732),
  NFR(a7b3, 928e),
  NFR(a815, 92be),
  NFR(a865, 9312),
  NFR(a890, 937e),
  NFR(a8b9, 9282),
  NFR(a8f8, 93a2),
  NFR(a923, 974a),
  NFR(a98d, 93d2),
  NFR(a9e5, 9426),
  NFR(aa0e, 92e2),
  NFR(aa41, 92d6),
  NFR(aa82, 93de),
  NFR(aade, 943e),
  NFR(ab07, 944a),
  NFR(ab3b, 9492),
  NFR(ab64, 9486),
  NFR(ab8f, 949e),
  NFR(abd2, 950a),
  NFR(ac00, 9522),
  NFR(ac2b, 94da),
  NFR(ac5a, 9546),
  NFR(ac83, 9552),
  NFR(acb3, 95ca),
  NFR(acf0, 97b6),
  NFR(ad1b, 95b2),
  NFR(ad5e, 9a4a),
  NFR(adad, 95d6),
  NFR(adde, 961e),
  NFR(ae07, 95ee),
  NFR(ae32, 9642),
  NFR(ae74, 965a),
  NFR(aeb4, 9666),
  NFR(aedf, 9576),
  NFR(af14, 967e),
  NFR(af3f, 96de),
  NFR(af72, 97f2),
  NFR(afa3, 955e),
  NFR(afce, 9786),
  NFR(affb, 968a),
  NFR(b026, 976e),
  NFR(b051, 96c6),
  NFR(b07a, 958e),
  NFR(b106, 970e),
  NFR(b139, 9762),
  NFR(b1e5, 9852),
  NFR(b236, 96f6),
  NFR(b283, 9a86),
  NFR(b2da, 9906),
  NFR(b305, 9a7a),
  NFR(b32e, 98ca),
  NFR(b37a, 995a),
  NFR(b3a5, 985e),
  NFR(b40a, 98ee),
  NFR(b457, 9912),
  NFR(b482, 9a32),
  NFR(b4ad, 994e),
  NFR(b4e5, 99f6),
  NFR(b510, 9a92),
  NFR(b55a, 9a9e),
  NFR(b585, 9ab6),
  NFR(b5d5, 99ea),
  NFR(b62b, 9a1a),
  NFR(b656, 99ae),
  NFR(b698, 98b2),
  NFR(b6c1, 988e),
  NFR(b6ee, 9a02),
  // ---- The Wrecked Ship and Maridia, played as a player would: walking, jumping, shooting, a
  // charged shot, missiles, then rolling and laying bombs. Each room is entered by the door that
  // leads to it (#, by the door's address), from the area's debug station 16.
  {"ship-off-ce8a", 3, 0, 0, -1, kWalkDir("LEFT")},
  SHIP_OFF("ship-off-caf6", "a1bc", kClimb),
  SHIP_ON("ship-on-caf6", "a1bc", kClimb),
  SHIP_ON("ship-on-ca52", "89ee", kWalkDir("RIGHT")),
  SHIP_ON("ship-on-cb8b", "a2dc", kWalkDir("RIGHT")),
  SHIP_ON("ship-on-cc27", "a27c", kWalkDir("LEFT")),
  SHIP_ON("ship-on-cc6f", "a21c", kClimb),
  {"phantoon-power-bomb", 3, 0, 1, -1, "-*30 @ " kPowerBombs},
  {"maridia-d1dd", 4, 16, 0, -1, kWalkDir("RIGHT")},
  MARIDIA("maridia-d017", "a444", kClimb),
  MARIDIA("maridia-d0b9", "a474", kClimb),
  MARIDIA("maridia-cefb", "a39c", kClimb),
  MARIDIA("maridia-cf54", "911a", kWalkDir("RIGHT")),
  MARIDIA("maridia-cf80", "a348", kWalkDir("RIGHT")),
  MARIDIA("maridia-d54d", "a714", kClimb),
  MARIDIA("maridia-d57a", "a720", kClimb),
  MARIDIA("maridia-d898", "a858", kClimb),
  MARIDIA("maridia-d433", "a5c4", kWalkDir("RIGHT")),
  MARIDIA("maridia-d408", "a5ac", kClimb),
  MARIDIA("maridia-d913", "a870", kWalkDir("RIGHT")),
  MARIDIA("maridia-d9fe", "a75c", kWalkDir("RIGHT")),
  MARIDIA("maridia-d5a7", "a4c8", kWalkDir("RIGHT")),
  MARIDIA("maridia-d7e4", "a918", kWalkDir("RIGHT")),
  MARIDIA("maridia-da60", "a840", kWalkDir("LEFT")),
  {"botwoon-power-bomb", 4, 0, 1, -1, "-*30 @ " kPowerBombs},
  {"draygon-power-bomb", 4, 0, 2, -1, "-*30 @ " kPowerBombs},
  // ---- The screens that are not a room: the pause screen, the game over, the escape's
  // timer and the message boxes.
  {"pause-tourian", 5, 16, 0, -1, kPauseScript},
  {"game-over", 5, 16, 0, -1, "-*60 ! -*500 @ -*200 @ UP*3 -*60 A*3 -*200 @ A*3 -*300 @"},
  {"escape-tourian", 5, 16, 0, -1, "-*30 @ $2 -*120 @ #aae0 " kRoomKeys " -*200 @"},
  {"ceres-boom", 5, 16, 0, -1, "#ab58 -*30 @ =32 -*400 @ -*400 @ -*400 @ -*400 @"},
  {"message-item", 0, 16, 0, -1, "-*60 @ %5 -*120 @ A*3 -*60 @ %1 -*120 @ A*3 -*60 @"},
  {"message-save", 1, 0, 0, -1, "-*60 @ %28 -*120 @ A*3 -*120 @ A*3 -*120 @ A*3 -*60 @"},
  {"boss-bomb-torizo-full", 0, 0, 1, -1, kBombTorizoFull},
  {"boss-spore-spawn-x", 1, 0, 1, -1, "-*30 @ >128,715 -*10 " kSelectSupers "(<40,5 R+B*2 -*6 @)x300"},
  {"boss-spore-spawn-full", 1, 0, 1, -1, kSporeSpawnFull},
  BOSS("bomb-torizo", 0, 1),
  BOSS("spore-spawn", 1, 1),
  BOSS("kraid", 1, 2),
  {"boss-kraid-full", 1, 0, 2, -1, kKraidFull},
  BOSS("crocomire", 2, 1),
  BOSS("ridley", 2, 2),
  {"boss-ridley-full", 2, 0, 2, -1, kRidleyFull},
  {"boss-ridley-death", 2, 0, 2, -1, kRidleyDeath},
  {"ceres-ridley-full", 5, 16, 0, -1, kCeresRidley},
  BOSS("phantoon", 3, 1),
  {"boss-phantoon-full", 3, 0, 1, -1, kPhantoonFull},
  BOSS("botwoon", 4, 1),
  {"boss-botwoon-full", 4, 0, 1, -1, kBotwoonFull},
  BOSS("draygon", 4, 2),
  {"boss-draygon-full", 4, 0, 2, -1, kDraygonFull},
  BOSS("mother-brain", 5, 1),
  {"boss-mother-brain-full", 5, 0, 1, -1, kMbFull},
  {"escape-zebes", 5, 16, 0, -1, kEscape},
  {"tourian-metroids-1", 5, 16, 0, -1, "#a984 " kMetroids},
  {"tourian-metroids-2", 5, 16, 0, -1, "#a9b4 " kMetroids},
  {"tourian-metroids-3", 5, 16, 0, -1, "#a9cc " kMetroids},
  {"tourian-metroids-4", 5, 16, 0, -1, "#a9e4 " kMetroids},
  ROOM("daae", "9222"),
  ROOM("dae1", "a984"),
  ROOM("db31", "a9b4"),
  ROOM("db7d", "a9cc"),
  ROOM("dbcd", "a9e4"),
  ROOM("dc19", "a9fc"),
  ROOM("dc65", "aa14"),
  ROOM("dcb1", "aa50"),
  ROOM("dcff", "aa98"),
  ROOM("dd2e", "aa68"),
  ROOM("dd58", "aae0"),
  ROOM("ddc4", "aab0"),
  ROOM("ddf3", "aa80"),
  ROOM("de23", "aabc"),
  ROOM("de4d", "aaf8"),
  ROOM("de7a", "ab10"),
  ROOM("dea7", "ab28"),
  ROOM("dede", "8b6e"),
  ROOM("df1b", "a99c"),
  ROOM("df45", "ab58"),
  ROOM("df8d", "ab4c"),
  ROOM("dfd7", "ab64"),
  ROOM("e021", "ab7c"),
  ROOM("e06b", "ab94"),
  ROOM("e0b5", "abac"),
  // The Golden Torizo's room, at its left door (Norfair's debug station 22).
  {"boss-golden-torizo", 2, 22, 0, -1, kBossScript},
  // The two fights played to their ends (as a player does, from the first blow to the room after),
  // by the boss's own state (the ( )W and ( )V loops), a shot every few frames to catch a layer or
  // a sprite that goes missing for a second. A shot holds smrun's frames, so these need more
  // than its default budget (tools/scenarios.sh's FRAMES: about 4 frames a shot).
  // Crocomire: Samus walks to him (he wakes at 224 pixels) and jumps at his mouth with super
  // missiles, kept 100 pixels from him (a player's footwork); pushed back along the floor to the
  // spikes (x 1480), over them (they crumble) and off the bridge (x 1600: his variable A is
  // 2 on) into the lava, he melts (A to 58) and his skeleton runs off to the left (x 500), then
  // Samus walks left after it (the bones fall; the death sets the boss bit) and out of the room
  // by its door (to the save room).
  {"boss-crocomire-full", 2, 0, 1, -1, "-*30 @ RIGHT*50 -*60 RIGHT*60 " kSelectSupers "`100 (A+R*20 (A+R+B*2 A+R*4)x5 -*40 @)Wf7a,1480 (@ A+R*4 @ A+R*4 @ A+R*4 @ A+R*4 @ A+R*4 (@ A+R+B*2 A+R*4)x5 (@ -*4)x10)Wfa8,2 `0 (@ -*3)Wfa8,58 (@ -*3)Vf7a,500 (@ LEFT*3)Wd82a,2 (@ -*3)x40 (@ LEFT*3)x20 -*30 @ #93de -*30 @ (@ -*3)x20 -*60 @"},
  // Power bombs in the morph ball in front of him.
  {"boss-crocomire-bombs", 2, 0, 1, -1, "-*30 @ RIGHT*50 -*60 RIGHT*60 -*60 " kSelectPowerBombs kMorph "R*10 @ R+B*2 (@ -*3)x60 -*100 (@ -*3)x20 R+B*2 (@ -*3)x60 (@ -*10)x20"},
  // The Golden Torizo: she walks to him (he wakes), super missiles (he catches them), then
  // charged shots, each way, until he dies and goes (his enemy slot's pointer is 0), and the
  // room's other door.
  {"boss-golden-torizo-full", 2, 22, 0, -1, "-*30 @ (@ RIGHT*4)Waf6,330 (@ -*4)x25 " kSelectSupers " (LEFT*4 @ R+B*3 (@ -*4)x6 RIGHT*4 @ R+B*3 (@ -*4)x6)x8 (LEFT*4 (@ B*4)x16 (@ -*4)x2 RIGHT*4 (@ B*4)x16 (@ -*4)x2)Vf8c,0 (@ -*4)Vf78,0 (@ -*4)x30 (@ RIGHT*4)x40 -*20 #9882 -*30 @ (@ -*4)x20 -*60 @"},
  {"pause-crateria", 0, 16, 0, -1, kPauseScript},
};
enum { kScenarioCount = sizeof(kScenarios) / sizeof(kScenarios[0]) };

// What smrun's KEYS held as the cartridge started chose.
static uint32_t calls;
static uint16_t first_scenario, scenario_count;
static bool silent;
static uint8_t order[kScenarioCount];
static int order_count, current;

static enum {
  kBooting,
  kMenus,    // the title to the room map and the spawn
  kLoading,  // until Samus has her controls
  kScript,
  kFinished,
} phase;
static uint32_t phase_frames;
static bool shot_pending, reset_pending, spawned, door_pending, door_waiting, mortal, hurt;
static int shot_count;

// The script's place.
static const char *script;
static struct { const char *start; int left; } loops[4];
static int follow;
static int loop_depth;
static uint16_t held_keys;
static int held_left;

static const char *const kKeyNames[] = {"A", "B", "SELECT", "START", "RIGHT", "LEFT", "UP", "DOWN",
                                        "R", "L"};
// The planet map's order of areas.
static const uint8_t kAreaOrder[6] = {0, 3, 5, 1, 4, 2};

// Whether |a| and |b| begin with the same |n| characters.
static bool Same(const char *a, const char *b, int n) {
  for (int i = 0; i < n; i++)
    if (a[i] != b[i])
      return false;
  return true;
}

static bool Picked(const char *name) {
#ifdef SM_SCENARIO_ONLY
  const char *list = SM_SCENARIO_ONLY;
  for (;;) {
    int len = 0;
    while (list[len] && list[len] != ',')
      len++;
    if (len && list[len - 1] == '*' ? Same(list, name, len - 1)
                                    : Same(list, name, len) && !name[len])
      return true;
    if (!list[len])
      return false;
    list += len + 1;
  }
#else
  return true;
#endif
}

static void Choose(void) {
  for (int i = 0; i < kScenarioCount; i++)
    if (Picked(kScenarios[i].name))
      order[order_count++] = i;
  // (For tools/scenarios.sh, to divide them.)
  for (int i = 0; i < order_count; i++)
    printf("scenario-list: %d %s\n", i, kScenarios[order[i]].name);
  int start = first_scenario < order_count ? first_scenario : order_count;
  int n = scenario_count ? scenario_count : order_count;
  if (start + n > order_count)
    n = order_count - start;
  memmove(order, order + start, n);
  order_count = n;
  printf("scenario: %d to run, of %d\n", order_count, kScenarioCount);
  phase = order_count ? kMenus : kFinished;
  if (!order_count)
    printf("scenario: all done\n");
}

static const Scenario *Current(void) {
  return &kScenarios[order[current]];
}

// Whether to press a button now: for a few frames, every so often.
static bool Pulse(int period) {
  return phase_frames % period < 4;
}

// From the title to the spawn, by the buttons a player would press.
static uint16_t Menus(void) {
  const Scenario *s = Current();
  switch (game_state) {
  case 1: case 2: case 4:
    // The title, the options and the file select: the tools on (L and R
    // with Start), then Start on, to a new game on the first file.
    if (!Pulse(40))
      return 0;
    return enable_debug ? kKeyStart : kKeyL | kKeyR | kKeyStart;
  case 5:
    if (menu_index == 6) {
      // The planet: to the area.
      if (!Pulse(12))
        return 0;
      return kAreaOrder[file_select_map_area_index] == s->area ? kKeyA : kKeyRight;
    }
    if (menu_index == 10) {
      // The area's map: the station, the loadout and the beams.
      if (!s->boss)
        load_station_index = s->station;
      Dev_Pick(s->beams, s->boss);
      if (phase_frames % 12 >= 4 || phase_frames < 30)
        return 0;
      spawned = true;
      return kKeyA;
    }
    return 0;
  case 6: case 7: case 8: case 9:
    // (The map's A has started the load: not the title's demo.)
    if (spawned) {
      phase = kLoading;
      phase_frames = 0;
      spawned = false;
    }
    return 0;
  default:
    return 0;
  }
}

static int Number(const char **p) {
  int n = 0;
  while (**p >= '0' && **p <= '9')
    n = n * 10 + *(*p)++ - '0';
  return n;
}

// The next word of the script that holds buttons, or false at its end.
static bool NextWord(void) {
  for (;;) {
    while (*script == ' ')
      script++;
    switch (*script) {
    case 0:
      return false;
    case '@':
      script++;
      shot_pending = true;
      continue;
    case '>':
      // Samus to X,Y (pixels): for a place no station is near.
      script++;
      samus_x_pos = Number(&script);
      script++;
      samus_y_pos = Number(&script);
      samus_x_subpos = samus_y_subpos = 0;
      continue;
    case '<': {
      // Samus DX pixels from the first enemy, on the room's centre side of it and DY pixels
      // below its centre, turned toward it (held 2 frames): to fight a boss that moves about,
      // as a player would stand at its height.
      script++;
      int dx = Number(&script);
      script++;
      bool above = *script == '-';
      script += above;
      int dy = Number(&script);
      int x = enemy_data[0].x_pos;
      bool face_right = x >= 128;
      samus_x_pos = face_right ? x - dx : x + dx;
      samus_y_pos = enemy_data[0].y_pos + (above ? -dy : dy);
      samus_x_subpos = samus_y_subpos = 0;
      held_keys = 1 << (face_right ? 4 : 5);
      held_left = 2;
      return true;
    }
    case '~': {
      script++;
      bool above = *script == '-';
      script += above;
      int n = Number(&script);
      fx_base_y_pos = samus_y_pos + (above ? -n : n);
      continue;
    }
    case 'P':
      script++;
      samus_max_power_bombs = samus_power_bombs = 0;
      continue;
    case '&':
      script++;
      boss_bits_for_area[area_index] = Number(&script);
      continue;
    case '$':
      script++;
      timer_status = Number(&script);
      // (As Mother Brain's death does, before it sets the status to 2: the escape timer's
      // tiles into VRAM (the table is the game's, in bank $A6), and the handling of the timer.)
      if (timer_status) {
        CallSomeSamusCode(0xf);
        if (timer_status == 2) {
          // (Byte by byte: the table's entries are at odd addresses.)
          for (uint16_t t = 0xC4CB; ; t += 7) {
            const uint8_t *entry = RomPtrWithBank(0xa6, t);
            if (!(entry[0] | entry[1]))
              break;
            memcpy(gVramWriteEntry(vram_write_queue_tail), entry, 7);
            vram_write_queue_tail += 7;
          }
        }
      }
      continue;
    case '=':
      script++;
      game_state = Number(&script);
      continue;
    case '^':
      script++;
      *(uint16_t *)(g_ram + 0xF8C) = Number(&script);
      continue;
    case '%':
      script++;
      queued_message_box_index = Number(&script);
      continue;
    case 'H':
      script++;
      hurt = true;
      continue;
    case 'h':
      script++;
      hurt = false;
      continue;
    case '!':
      script++;
      mortal = true;
      samus_health = samus_reserve_health = 0;
      continue;
    case '_':
      // Samus's energy to N, no reserve and no refills (+ gives them back).
      script++;
      mortal = true;
      samus_health = Number(&script);
      samus_reserve_health = 0;
      continue;
    case '+':
      script++;
      mortal = false;
      continue;
    case '#': {
      script++;
      int door = 0;
      for (;; script++) {
        int c = *script;
        if (c >= '0' && c <= '9')
          c -= '0';
        else if (c >= 'a' && c <= 'f')
          c -= 'a' - 10;
        else
          break;
        door = door << 4 | c;
      }
      door_def_ptr = door;
      door_transition_function = (uint16_t)fnDoorTransitionFunction_HandleElevator;
      game_state = kGameState_9_HitDoorBlock;
      door_pending = true;
      // (No buttons while the door runs: see Scn_Keys.)
      held_keys = 0;
      held_left = 1;
      return true;
    }
    case ':': {
      // :ADDR=N: the game's 16-bit word at ADDR (hex) to N (decimal).
      script++;
      int addr = 0;
      for (;; script++) {
        int c = *script;
        if (c >= '0' && c <= '9')
          c -= '0';
        else if (c >= 'a' && c <= 'f')
          c -= 'a' - 10;
        else
          break;
        addr = addr << 4 | c;
      }
      script++;
      uint16_t v = Number(&script);
      memcpy(g_ram + addr, &v, 2);
      continue;
    }
    case 'G':
      // (Mother Brain's tank: its glass counts missile hits, to 18; see kMbFull.)
      script++;
      plm_room_arguments[39] += Number(&script);
      continue;
    case '(':
      script++;
      loops[loop_depth].start = script;
      loops[loop_depth++].left = 0;
      continue;
    case ')': {
      // )xN: N times; )WOFF,N: until the word at OFF (hex, from the game's RAM) is at least N,
      // or )VOFF,N: at most N (but not more than 1500 times: a fight that never gets there).
      int d = loop_depth - 1;
      char kind = script[1];
      script += 2;
      if (kind == 'x') {
        if (!loops[d].left)
          loops[d].left = Number(&script);
        else
          Number(&script);
        if (--loops[d].left)
          script = loops[d].start;
        else
          loop_depth--;
        continue;
      }
      int off = 0, n;
      for (;; script++) {
        int c = *script;
        if (c >= '0' && c <= '9')
          c -= '0';
        else if (c >= 'a' && c <= 'f')
          c -= 'a' - 10;
        else
          break;
        off = off << 4 | c;
      }
      script++;
      n = Number(&script);
      uint16_t v = *(uint16_t *)(g_ram + off);
      if (++loops[d].left < 1500 && (kind == 'W' ? v < n : v > n))
        script = loops[d].start;
      else
        loop_depth--;
      continue;
    }
    case '`':
      // ^N: Samus keeps N pixels from the first enemy (a player's footwork), 0 for not.
      script++;
      follow = Number(&script);
      continue;
    }
    held_keys = 0;
    while (*script && *script != '*') {
      int len = 0;
      while (script[len] && script[len] != '+' && script[len] != '*')
        len++;
      for (int i = 0; i < 10; i++)
        if (Same(script, kKeyNames[i], len) && !kKeyNames[i][len])
          held_keys |= 1 << i;
      script += len + (script[len] == '+');
    }
    script++;
    held_left = Number(&script);
    return true;
  }
}

static void Start(void) {
  const Scenario *s = Current();
  script = s->script;
  loop_depth = 0;
  held_left = 0;
  shot_count = 0;
  shot_pending = false;
  follow = 0;
  phase = kScript;
  door_pending = door_waiting = mortal = hurt = false;
  phase_frames = 0;
  printf("scenario: %s\n", s->name);
  printf("scenario-at: room %04x samus %d,%d\nscenario-fx: %02x\n", room_ptr, samus_x_pos, samus_y_pos, fx_type);
}

static void Finish(void) {
  printf("scenario: end %s\n", Current()->name);
  if (++current == order_count) {
    phase = kFinished;
    printf("scenario: all done\n");
  } else {
    // Back to the title, as a reset does.
    reset_pending = true;
    phase = kMenus;
    phase_frames = 0;
  }
}

// After a door: Samus on the screen, if the door left her off it (an elevator's door leaves
// her where the door's far side is, by the camera's reckoning of it, and a camera that can't
// reach the place she was put leaves her outside the room): where she would be at the door's
// far side, from the camera and the way she came (the transition's own walk is left out).
static void PlaceSamusOnScreen(void) {
  int sx = samus_x_pos - layer1_x_pos, sy = samus_y_pos - layer1_y_pos;
  if (sx >= 16 && sx < 240 && sy >= 16 && sy < 208)
    return;
  static const int8_t kInX[4] = {-72, 72, 0, 0}, kInY[4] = {24, 24, -48, 60};
  int d = door_direction & 3;
  printf("scenario-fix: samus off the screen at %d,%d (camera %d,%d), put by the door's direction %d\n",
         samus_x_pos, samus_y_pos, layer1_x_pos, layer1_y_pos, d);
  samus_x_pos = samus_prev_x_pos = layer1_x_pos + 128 + kInX[d];
  samus_y_pos = samus_prev_y_pos = layer1_y_pos + 112 + kInY[d];
  samus_x_subpos = samus_y_subpos = 0;
}

uint16_t Scn_Keys(uint16_t real) {
  // (The buttons at the start choose: see the top.)
  calls++;
  if (calls == 20)
    first_scenario = real;
  if (calls == 130)
    scenario_count = real & 0x1ff, silent = real >> 9 & 1;
  if (calls <= 130)
    return 0;
  if (calls == 131)
    Choose();
  phase_frames++;
  switch (phase) {
  case kMenus:
    return Menus();
  case kLoading:
    // Samus's arrival is over, and through her boss's door, if she has one
    // (the game gives her the controls then).
    if (game_state == kGameState_8_MainGameplay &&
        frame_handler_beta == (uint16_t)fnSamus_FrameHandlerBeta_Func17 && !Dev_SpawnPending()) {
      if (phase_frames > 20)
        Start();
    } else {
      phase_frames = 0;
    }
    return 0;
  case kScript:
    if (door_pending) {
      // The room's loading; an elevator's door leaves her controls off, as
      // tour.c's.
      if (game_state != kGameState_8_MainGameplay)
        return 0;
      CallSomeSamusCode(1);
      CallSomeSamusCode(0xb);
      // What Ceres's HDMA writes to BGMODE below the HUD is this, set to 9 by the elevator shaft's
      // door code: a door entered directly would leave 0, Mode 0, which the game never has.
      if (area_index == 6 && !reg_BGMODE_fake)
        reg_BGMODE_fake = 9;
      door_pending = false;
      door_waiting = true;
      held_keys = 0;
      held_left = 20;
    }
    if (door_waiting && !held_left) {
      // The room has settled (the camera is where it will be): Samus should be in it.
      door_waiting = false;
      PlaceSamusOnScreen();
      printf("scenario-at: room %04x samus %d,%d camera %d,%d pose %02x\nscenario-fx: %02x\n",
             room_ptr, samus_x_pos, samus_y_pos, layer1_x_pos, layer1_y_pos, samus_pose, fx_type);
    }
    if (!held_left && !NextWord()) {
      if (shot_pending) {
        // (The shot is of the frame after it.)
        held_keys = 0;
        held_left = 1;
      } else {
        Finish();
        return 0;
      }
    }
    held_left--;
    return held_keys;
  default:
    return 0;
  }
}

void Scn_BeforeFrame(void) {
  if (reset_pending) {
    // As the game's own reset: main.c goes on from there.
    reset_pending = false;
    game_state = 0xffff;
    Input_Reset();
  }
  if (phase == kScript && follow) {
    int want = *(uint16_t *)(g_ram + 0xF7A) - follow;
    if (samus_x_pos - want > 12 || want - samus_x_pos > 12) {
      samus_x_pos = samus_prev_x_pos = want;
      samus_x_subpos = 0;
    }
  }
  if (phase == kScript && phase_frames % 30 == 0) {
    // (For reading a fight's progress off the log: the first three enemies and Samus.)
    EnemyBase *e = (EnemyBase *)(g_ram + 0xF78);
    EnemyBase *e1 = (EnemyBase *)(g_ram + 0xFB8), *e2 = (EnemyBase *)(g_ram + 0xFF8);
    printf("scenario-state: %d gs %x room %04x e0 %04x %d,%d hp %d v%x ci %x it %d pr %x vB %x vC %x vD %x e1 %04x hp %d e2 %04x hp %d samus %d,%d bb %x\n",
           phase_frames, game_state, room_ptr, e->enemy_ptr, e->x_pos, e->y_pos, e->health, *(uint16 *)(g_ram + 0xFA8), e->current_instruction, e->instruction_timer, e->properties, *(uint16 *)(g_ram + 0xFAA), *(uint16 *)(g_ram + 0xFAC), *(uint16 *)(g_ram + 0xFAE),
           e1->enemy_ptr, e1->health, e2->enemy_ptr, e2->health, samus_x_pos, samus_y_pos, *(uint16_t *)(g_ram + 0xD82A));
  }
  if ((phase == kLoading || phase == kScript) && !mortal) {
    if (!hurt)
      samus_health = samus_max_health;
    samus_missiles = samus_max_missiles;
    samus_super_missiles = samus_max_super_missiles;
    samus_power_bombs = samus_max_power_bombs;
  }
}

bool Scn_AfterFrame(void) {
  if (!shot_pending || phase != kScript)
    return false;
  shot_pending = false;
  printf("tour: shot %s-%d\n", Current()->name, ++shot_count);
  return !silent;
}
