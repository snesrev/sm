# sm

Our discord server is: https://discord.gg/AJJbJAzNNJ

Early version. It has bugs and the code is messy.

When running, it runs an emulated version in the background and compares the ram state every frame. If it detects a mismatch, it saves a snapshot in saves/ and displays a counter on screen counting down from 300. Please submit these bug snapshots on discord so that they can be fixed.

## Building
You must self-build for now. Steps for 64-bit Windows:<br>
<s>(0) Download [Python](https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe) and install with "Add to PATH" checked</s><br>
(1) Click the green button "Code > Download ZIP" on the github page and extract the ZIP<br>
(2) Place your USA rom named sm.sfc in that folder<br>
(3) Download [TCC](https://github.com/FitzRoyX/tinycc/releases/download/tcc_20230519/tcc_20230519.zip) and [SDL2](https://github.com/libsdl-org/SDL/releases/download/release-2.28.2/SDL2-devel-2.28.2-VC.zip) and extract each ZIP into the "third-party" subfolder<br>
<s>(4) Double click "extract_assets.bat" in the main dir. This will create sm_assets.dat.</s><br>
(5) Double-click "run_with_tcc.bat" in the main dir. This will create sm.exe and run it.<br>
(6) Configure with sm.ini in a text editor like notepad++<br>

For other platforms and compilers, see: https://github.com/snesrev/sm/blob/main/BUILDING.md
