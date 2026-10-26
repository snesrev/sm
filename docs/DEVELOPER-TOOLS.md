# Developer tools

The cartridge can start Samus anywhere, with any loadout, to test a room, a
fight or a weapon without playing up to it. It's the developers' own debug
mode: the game keeps a switch (`enable_debug`) that makes the load-game map
list every load station, the developers' spawn points among them. The
cartridge's buttons turn it on, and add a loadout, the whole map and the
bosses' doors.

**Turning it on.** On the title screen or the file select, hold L and R and
press Start. A sound tells you, and it lasts until the console is reset
(Start + Select + L + R in the menus resets it, too). Nothing in the demos or
in play can turn it on.

**Spawning.** Pick a file and start it, as usual. A saved file goes to the map
of Planet Zebes as ever; with the tools on, a new file skips its intro and goes
there too (to play the intro instead, hold L from Start until the screen fades).

| Screen      | Buttons                                                                 |
|-------------|-------------------------------------------------------------------------|
| Planet Zebes | Up and Left, Down and Right or Select: another area. A or Start: that area |
| Area map    | Select: the next station; after an area's last, its bosses; then the first again. D-pad: scroll the map. A or Start: spawn. B: back to the areas |
|             | L: Samus as saved, or with ALL ITEMS                                    |

The map shows every room of the area, as if its map station had been used
(your file keeps what it had explored: the map is only dressed up while it's
up), and every station, visited or not: the area's save stations and
elevators, then the **debug stations**, numbered from 16, that no file ever
had. The one picked is the one the blinking marker is on. Under the map, a
line names it (or its room's number, in hex, for the unnamed), then a line
for Select (next) and L (the loadout, as saved or ALL ITEMS), and one for the
other buttons, each in a row of its own so they stay readable on the GBA's
squashed screen. For a boss's door the marker stays at the station that the
spawn loads, and the line names the boss.

<p align="center">
  <img src="images/dev-map.png" width="240" alt="The whole map of an area with a station picked, and the loadout under it">
  <img src="images/dev-crocomire.png" width="240" alt="Crocomire, spawned into with all items">
  <img src="images/dev-draygon.png" width="240" alt="Draygon's room, spawned into with all items">
</p>

**The debug stations.** The game's own, in the order the map steps through
them (area, number: the room, by its pointer; it has each of the bosses' rooms
next to one, but only the Golden Torizo's inside it):

| Station | Room | Where |
|---------|------|-------|
| Crateria 16 | 91F8 | the landing site, at Samus's ship |
| Crateria 17 | 94FD | |
| Brinstar 16 | 9AD9 | |
| Brinstar 17 | A56B | next to Kraid: a door of Kraid's room leads here |
| Brinstar 18 | 9D19 | |
| Norfair 16 | A7DE | |
| Norfair 17 | A923 | next to Crocomire: a door leads into his room |
| Norfair 18 | B37A | next to Ridley |
| Norfair 19 | AA82 | next to Crocomire, by his room's other door |
| Norfair 20 | B236 | |
| Norfair 21 | B283 | the Golden Torizo's room, but the developers' spot is outside the room: the map spawns at 22's instead |
| Norfair 22 | B283 | the Golden Torizo's room, at its left door |
| Wrecked Ship 16 | CA08 | |
| Wrecked Ship 17 | CC6F | next to Phantoon |
| Maridia 16 | D1DD | |
| Maridia 17 | D78F | next to Draygon |
| Maridia 18 | D617 | next to Botwoon |
| Maridia 19 | D48E | |
| Tourian 16, 17 | DDF3 | next to Mother Brain |

**The bosses.** After an area's last station, the map goes on to its bosses'
doors (the game's own doors, from the side Samus comes in by): Bomb Torizo
(Crateria), Spore Spawn and Kraid (Brinstar), Crocomire and Ridley (Norfair),
Phantoon (Wrecked Ship), Botwoon and Draygon (Maridia) and Mother Brain
(Tourian). The spawn loads the area's last station, and when Samus has her
controls, goes through the door as if she had walked into it. (Crocomire's is
the hatch in the ceiling of his room, as in the game. As there, beams don't
hurt him: missiles in his mouth push him back.)

**Loadout.** L toggles between AS SAVED (what your file has) and ALL ITEMS:
1499 energy, 400 reserve, 230 missiles, 50 super missiles and 50 power bombs,
every item, and the charge, ice, wave and plasma beams (the spazer collected,
but plasma takes it off, as when the game gives you a beam or in its equipment
screen: with both on, no shots showed in testing). ALL ITEMS makes every boss
alive again, so that a file that has beaten them can fight them.

A spawn with a loadout can't save: it would write the loadout over your file,
so saving does nothing until you spawn again AS SAVED, or reset. Dying
goes back to the map, to spawn again.

**In the game.** Hold L, R and Select, and press: A to turn invincibility on
or off, B to refill energy and ammunition, Left to freeze the enemies or let
them go, Up to die (and so to go back to the map).

The SNES debug pad's own shortcuts (Select held with L or R, then A, B or
X: they would turn on an interrupt that menus have no handler for, or take
Samus's missiles away) are off.

These are the game's own debug features that the buttons reach (there's more
in the code: a Draygon that a second controller moves, a scroll position
saved and loaded, a demo recorder's pause). See the comments in
[gba/dev_tools.c](../gba/dev_tools.c) and the
[porting guide](PORTING-GUIDE.md).
