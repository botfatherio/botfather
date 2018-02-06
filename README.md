# BotFather
A scriptable environment for running browser related bot for automatisation or website testing.
Scripts can be written in Javascript. They call functions of BotFathers API allowing them to control the inbuild browser.
There is also a vision API which makes it possible to react to things happening visually in the browser.

## Gettings started (Arch Linux)
- Install the following packages with Pacman or Yaourt: ```sudo pacman -S cmake qtcreator xlib opencv``` or ```yaourt -S cmake qtcreator xlib opencv```.

### Chromium Embedded Framework (CEF)
CEF powers bot fathers browser thus it's required to build botfather.
- Download the latest Chromium Embedded Framework (CEF) Linux 64bit from http://opensource.spotify.com/cefbuilds/index.html
- Unpack the just downloaded CEF files to ```/opt/cef/``` (because that where botfather expects the cef headers to be for simplixity (atleast for now))
- (There is no need to build CEF because it's a binary distribution)
- (```/opt/cef/Resources/```, ```/opt/cef/Debug/``` and ```/opt/cef/Release/``` contain all files we need to bundle with botfather)

### Build the BotFather Helper
- Open botfather_helper/botfather_helper.pro with QtCreator and configure the project.
- Try to build the project. It will fail. But we need Qt to create the build folder(s).
- Now copy the contents of ```/opt/cef/Resources/``` to the build folder.
- Building the BotFather helper should work now as expected.

### Build BotFather
- Open botfather/botfahter.pro with QtCreator and configure the project.
- Try to build the project. It will fail. But we need Qt to create the build folder(s).
- Now copy the contents of ```/opt/cef/Resources/``` and (depending on your desired build) ```/opt/cef/Debug/``` or ```/opt/cef/Release/``` to the build folder.
- Building the BotFather should work now as expected.

### Add/Update Flashplayer
- Download the latest PPAPI for Opera and Chrome for Linux 64bit as tar.gz from https://get.adobe.com/de/flashplayer/otherversions/ and extract it's contents.
- Place the ```libpepflashplayer.so``` in the BotFather build output directory.
- The version number can be found in the ```readme.txt``` bundled with the ```libpepflashplayer.so``` file. 
- Adjust the flashplayer version specified in ```browser/browser_app.cpp``` where it says ```command_line->AppendSwitchWithValue("ppapi-flash-version", "YOUR.VERSION.HERE");```.

### Run BotFather
Before you can run BotFather you need to copy the botfather_helper binary from the respective build dir (depending on the build type (debug/release)) to the BotFather binary.
They have to be in the same folder and they need to be deployed together.

## Getting started (Microsoft Windows)
- Install CMAKE.
- Install Microsoft Visual Studio 2015 with C++ support and the latest toolset updates.
- Install Qtcreator and a Qt version.
- Install Qtaddin for MS VS 2015. VS -> Tools -> Extensions and Updates -> Online -> (search) qt -> (install) QtPackage. Restart System.
- Select Qt Version. VS -> QT5 -> Qt Options -> (Qt Versions) Add -> (eg. C:\Qt\5.10.0\msvc2015_64) matching the compiler and platform.

### Chromium Embedded Framework (CEF)
CEF powers bot fathers browser thus it's required to build botfather.
- Download the latest Chromium Embedded Framework (CEF) Linux 64bit from http://opensource.spotify.com/cefbuilds/index.html
- Unpack CEF to C:/CEF/your_cef_version_here
- Open CMAKE. Choose 64bit ms2015 compiler.
- Make the cef version dir the source and build dir.
- Configure (twice) and generate.
- Open the project with VS 2015.
- Change VS -> Project -> libcef_dll_wrapper Properties -> C/C++ -> Code Generation -> Runtime Library to MDd (debug) and MD (release).
- Build libcef_dll_wrapper both debug and release.

### Build the BotFather Helper (on Windows)
- Create new Project. VS -> File -> New -> Project -> Templates -> Visual C++ -> General -> Empty Project
- Adjust project props:
- C/C++ -> General -> Additional Include Directories -> ```C:\CEF\your_cef_version_here``` (release & debug)
- Linker -> General -> Additional Library Directories -> ```C:\CEF\your_cef_version_here\Release``` and ```C:\CEF\your_cef_version_here\libcef_dll_wrapper\Release``` (release)
- Linker -> General -> Additional Library Directories -> ```C:\CEF\your_cef_version_here\Debug``` and ```C:\CEF\your_cef_version_here\libcef_dll_wrapper\Debug``` (debug)
- Linker -> Input -> Additional Dependencies -> ```libcef_dll_wrapper.lib``` and ```libcef.lib``` (release & debug)
- Linker -> System -> SubSystem -> Windows (/SUBSYSTEM:WINDOWS) (release & debug)
- Copy ```compatibility.manifest``` from ```C:\CEF\your_cef_version_here\tests\cefsimple\``` into the botfather_helper project directory.
- Goto VS -> Manifest Tool -> Input Output -> Additional manifest file and Add the compatibility manifest.
- Compile
- Check whether the manifest has been added: (see) VS -> File... -> (choose exe) -> Open with (use the open buttons dropdown) -> Resource Editor

## Gettings started (Apple MacOS)
...to be added...

## Troubleshooting
##### LaunchProcess: failed to execvp: ./botfather_helper
This means you forgot to copy the botfather_helper binary from it's build folder to the bots folder from which you're probably trying to run a bot binary.
Place botfather_helper binary matching platform and build type (release/debug) next to your bots binary to fix the problem.