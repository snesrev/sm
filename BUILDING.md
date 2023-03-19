# Requirements
  * A Super Metroid rom (Make sure to rename it to `sm.smc`)
  * libsdl2-dev
  
 For Linux/MacOS you must install these for your desired OS:
 * Ubuntu/Debian: `sudo apt install libsdl2-dev`
 * Fedora Linux: `sudo dnf in sdl2-devel`
 * Arch Linux: `sudo pacman -S sdl2`
 * macOS: `brew install sdl2`

# Windows

## Building with MSYS2

Dependencies and requirements:

Note: *Make sure you're using MINGW64, otherwise it won't work.*
  * The `libsdl2-dev` library
  * [MSYS2](https://www.msys2.org)

1. Install MSYS2 on your machine.
2. Clone the SM repository. `git clone https://github.com/snesrev/sm`
3. Place the copy of your rom in the main directory.
4. Install the necessary dependencies by inputting this command in the terminal.
```sh
pacman -S mingw-w64-x86_64-SDL2 && pacman -S make && pacman -S mingw-w64-x86_64-gcc
```
5. Type `sdl2-config --cflags`, it should output:
```sh
-IC:/msys64/mingw64/include/SDL2 -Dmain=SDL_main
```
6. After that type `sdl2-config --libs`, should output:
```sh
-LC:/msys64/mingw64/lib -lmingw32 -mwindows -lSDL2main -lSDL2
```
NOTE: *Make sure you're in the `sm` directory.*

After you've done installing everything, cd to `sm` folder. Type `make`
In order to speed up the compilation, type `make -j16`

## Building with Visual Studio

Dependencies and requirements:
 * The `libsdl2-dev` library, which is automatically installed with NuGet.
 * [Visual Studio Community 2022](https://visualstudio.microsoft.com)

Download VS installer. On installer prompt, make sure you're on "Workloads" and check `Desktop Development with C++` this will install the necessary deps for compilation.

1. Open `sm.sln` solution.
2. Change the build target from `Debug` to `Release`
3. Build the solution.

## Building with Tiny C Compiler

 Dependencies and requirements:
 * You'll need [TCC](https://github.com/FitzRoyX/tinycc/releases/download/tcc_20221020/tcc_20221020.zip) and [SDL2](https://github.com/libsdl-org/SDL/releases/download/release-2.24.1/SDL2-devel-2.24.1-VC.zip) in order to compile using TCC.

1. Unzip both TCC and SDL and place them in `third_party` folder.
2. Double click `run_with_tcc.bat`
3. Wait for it to compile and the game will automatically boot-up.

# Linux/MacOS

Cd to your SM root folder and open the terminal and type:
```sh
make
```

For more advanced usage:
```sh
make -j$(nproc) # run on all core
make clean all  # clear gen+obj and rebuild
CC=clang make   # specify compiler
```

# Nintendo Switch

Dependencies and requirements:

  * The `switch-sdl2` library
  * MSYS2
  * [DevKitPro](https://devkitpro.org/wiki/Getting_Started)
  * [Atmosphere](https://github.com/Atmosphere-NX/Atmosphere)
  
 Note: *You must have MINGW64 open and affirm you're in the `sm` root folder.*

1. Make sure you've installed DevKitPro and Atmosphere.
2. Type `(dkp-)pacman -S git switch-dev switch-sdl2 switch-tools` to install the Switch SDL2 deps.
3. Now go to the `switch` folder inside MSYS2 `cd platform/switch/` 
4. Type `make` to build the game.
5. Copy the `.ini`, `nro` and the `smc` file and put them in your Switch.

**OPTIONAL STEPS**

```sh
make -j$(nproc) # To build using all cores
nxlink -s zelda3.nro # To test the build directly onto the Switch
```
