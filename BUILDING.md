# Requirements
  * A Super Metroid rom (Make sure to rename it to 'sm.smc')
  * libsdl2-dev
  
 For Linux/MacOS you must install these for your desired OS:
 * Ubuntu/Debian: `sudo apt install libsdl2-dev`
 * Fedora Linux: `sudo dnf in sdl2-devel`
 * Arch Linux: `sudo pacman -S sdl2`
 * macOS: `brew install sdl2`

# Windows

## Building with MSYS2

Dependencies and requirements:
  * The `libsdl2-dev` library
  * [MSYS2](https://www.msys2.org)

1. Install MSYS2 on your machine.
2. Clone the SM repository. `git clone https://github.com/snesrev/sm`
3. Place the copy of your rom in the main directory.
4. Install `libsdl2-dev` with `sdl2-config --cflags`
5. After that it outputs `-IC:/msys64/mingw64/include/SDL2 -Dmain=SDL_main`, type `sdl2-config --libs`, it should output `-LC:/msys64/mingw64/lib -lmingw32 -mwindows -lSDL2main -lSDL2`
6. Now install Make and GCC, type `pacman -S make && pacman -S mingw-w64-x86_64-gcc`

NOTE: *Make sure you're in the `sm` directory.*

After you've done installing everything, open MSYS2 and cd to `sm` foler. Type `make`
In order to speed up the compilation, type `make -j16`

## Building with Visual Studio

Dependencies and requirements:
 * The `libsdl2-dev` library, which is automatically installed with NuGet.
 * [Visual Studio Community 2022](https://visualstudio.microsoft.com)

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
