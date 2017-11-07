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
...to be added...

## Gettings started (Apple MacOS)
...to be added...

## Troubleshooting
##### LaunchProcess: failed to execvp: ./botfather_helper
This means you forgot to copy the botfather_helper binary from it's build folder to the bots folder from which you're probably trying to run a bot binary.
Place botfather_helper binary matching platform and build type (release/debug) next to your bots binary to fix the problem.