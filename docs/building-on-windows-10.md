# Microsoft Windows 7, 8, 10 build instructions

### Step 1: Install tools and dependencies

- Install CMAKE.
- Install Microsoft Visual Studio 2015 with C++ support and the latest toolset updates.
- Install Qtcreator and a Qt version.

### Step 2: Chromium Embedded Framework

- Download Chromium Embedded Framework (CEF) Windows 64-bit from http://opensource.spotify.com/cefbuilds/index.html
- Unpack CEF to a persistent location and make the environment variable `CEF_ROOT` point to it
- Open CMAKE. Choose 64bit ms2015 compiler.
- Make the cef version dir the source and build dir.
- Configure (twice) and generate.
- Open the project with VS 2015.
- Change VS -> Project -> libcef_dll_wrapper Properties -> C/C++ -> Code Generation -> Runtime Library to MDd (debug) and MD (release).
- Build libcef_dll_wrapper both debug and release.

### Step 3: Build OpenCV 3

- Download the latest OpenCV Win Pack from https://opencv.org/releases.html
- Run/Unpack it (it's probably a self extracting zip archive).
- Run CMAKE over it. You need to create an extra `build` folder.
- Configure. Make sure opencv world is selected. Configure. Generate.
- Open the generated OpenCV Project with Visual Studio 2015.
- Build ALL_BUILD both debug and release. Done.
- Make the environment variable `OPENCV_ROOT` point to the build. (should also contain `include/`)

### Step 4: Build BotFather

- Open `package.pro` with QtCreator and configure the project.

### Deploy Botfather

- The contents of `botfather/BuildOutput` and `webhelper/BuildOutput` must be combined before shipping (or running)
- Get the latest precompiled openssl dlls from https://indy.fulgan.com/SSL/ (scroll down) and ship them with botfather 
- [Install python 3](https://www.digitalocean.com/community/tutorials/how-to-install-python-3-and-set-up-a-local-programming-environment-on-windows-10) to use the tools in the `botfather-repo` repository