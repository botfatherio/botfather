# BotFather
With botfather one can write crossplatform bots for Android, Desktop and Web (browser) applications in JavaScript.
For this purpose Botfather provides several JavaScript APIs featuring image matching and more.
Furthermore scripts can be turned into crossplatform binaries using Botfather Binary Generator Tool.

------
## Build instructions for Arch Linux or Ubuntu 18.04

### Install third party software

##### From your repository
- On Ubuntu 18.04: ```sudo apt install git qtdeclarative5-dev qtmultimedia5-dev qtscript5-dev qtcreator libqt5x11extras5-dev libopencv-*```
- On Arch Linux: ```sudo pacman -S qtcreator xlib opencv```

##### Chromium Embedded Framework from source
- Download CEF Linux 64bit from http://opensource.spotify.com/cefbuilds/index.html
- Unpack the just downloaded CEF files to ```/opt/cef/``` (thats where botfather will expect CEF)
- Config the source with `cmake-gui` (2x configure, 1x generate)
- Compile CEF with `make -j 9`. It will eventuelly fail AFTER the dll wrapper has been build. Thats okay

### Build the BotFather Helper
- Open botfather_helper/botfather_helper.pro with QtCreator and configure the project.
- Try to build the project. It will fail. But we need Qt to create the build folder(s).
- Now copy the contents of ```/opt/cef/Resources/``` to the build folder.
- Building the BotFather helper should work now as expected.

### Build BotFather
- Open botfather/botfahter.pro with QtCreator and configure the project.
- Try to build the project. It will fail. But we need Qt to create the build folder(s).
- Now copy the contents of ```/opt/cef/Resources/``` and ```/opt/cef/Debug/``` or ```/opt/cef/Release/``` to the build folder.
- Building the BotFather should work now as expected.

------
### Build instructions for Microsoft Windows 7, 8, 10
- Install CMAKE.
- Install Microsoft Visual Studio 2015 with C++ support and the latest toolset updates.
- Install Qtcreator and a Qt version.

### Install third party software

##### From the internet
- Install CMAKE.
- Install Microsoft Visual Studio 2015 with C++ support and the latest toolset updates.
- Install Qtcreator and a Qt version.

##### Chromium Embedded Framework from source
- Download Chromium Embedded Framework (CEF) Windows 64-bit from http://opensource.spotify.com/cefbuilds/index.html
- Unpack CEF to C:/CEF/your_cef_version_here
- Open CMAKE. Choose 64bit ms2015 compiler.
- Make the cef version dir the source and build dir.
- Configure (twice) and generate.
- Open the project with VS 2015.
- Change VS -> Project -> libcef_dll_wrapper Properties -> C/C++ -> Code Generation -> Runtime Library to MDd (debug) and MD (release).
- Build libcef_dll_wrapper both debug and release.

##### Build OpenCV from source
- Download the latest OpenCV Win Pack from https://opencv.org/releases.html
- Run/Unpack it (it's probably a self extracting zip archive).
- Place OpenCV in ```C:\OPENCV\your_opencv_version```
- Run CMAKE and select ```C:\OPENCV\your_opencv_version\source``` as source folder and ```C:\OPENCV\your_opencv_version\build``` as build folder.
- Configure. Make sure opencv world is selected. Configure. Generate.
- Open the generated OpenCV Project with Visual Studio 2015.
- Build ALL_BUILD both debug and release. Done.

### Build the BotFather Helper
- Open botfather_helper/botfather_helper.pro with QtCreator and configure the project.
- Eventually adjust the ```INCLUDEPATH``` to match your cef version.
- Try to build the project. It will fail. But we need Qt to create the build folder(s).
- Now copy the contents of ```/CEF/your_version_here/Resources/``` to the build folder.
- Building the BotFather helper should work now as expected.

### Build BotFather
- Open botfather/botfahter.pro with QtCreator and configure the project.
- Eventually adjust the ```INCLUDEPATH``` to match your cef version.
- Try to build the project. It will fail. But we need Qt to create the build folder(s).
- Now copy the contents of ```/CEF/your_version_here/Resources/``` and ```/CEF/your_version_here/Debug/``` or ```/CEF/your_version_here/Release/``` to the build folder.
- Building the BotFather should work now as expected.

### Deploy Botfather
- Get the latest precompiled openssl dlls from https://indy.fulgan.com/SSL/ (scroll down) and ship them with botfather

------
## Troubleshooting / FAQ
##### LaunchProcess: failed to execvp: ./botfather_helper
This means you forgot to copy the botfather_helper binary from it's build folder to the bots folder from which you're probably trying to run a bot binary.
Place botfather_helper binary matching platform and build type (release/debug) next to your bots binary to fix the problem.

##### Browser is blank / nothing works after CEF Update
Make sure you rebuild(!) the botfather_helper and(!) botfather. Also make sure you copy the updated botfather_helper correctly.

##### How to choose a new CEF version
See what branches are supported and stable on [CEF Release Branches](https://bitbucket.org/chromiumembedded/cef/wiki/BranchesAndBuilding.md#markdown-header-release-branches)
CEF releases considered stable mostly aren't. Check what CEF release other project like CefSharp use in their latest stable release. [CefSharp Releases](https://github.com/cefsharp/CefSharp/releases)
