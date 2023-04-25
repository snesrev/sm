# sm

Sm is a PC port of Super Metroid compatible with Windows and Linux systems.

Currently, it's in an early version.

## Usage

To build:
> Note that the name of the SDL2 package varies between Linux distros. Th below exmaple is for Debian.
```bash
apt install libsdl2-dev
git clone --recursive https://github.com/snesrev/sm
cd sm
make
```
Once done, place a Super Metroid ROM into the `sm` folder and rename it to "sm.smc".

If building on another Linux distro, macOS or Windows, see [BUILDING.md](BUILDING.md).

#
Consider joining the Discord server: https://discord.gg/AJJbJAzNNJ
