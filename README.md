# Botfather

With Botfather one can write crossplatform bots for Android, Desktop and Web (browser) applications in JavaScript.
For this purpose Botfather provides several JavaScript APIs featuring image matching and more.
Furthermore scripts can be turned into crossplatform binaries using the Botfather Binary Generator Tool (bingen for short).

## Building on Linux

1. Install gcc and cmake (e.g. `sudo pacman -S gcc cmake`)
2. Install `Qt5`, including the Qt5Script module (e.g. `sudo pacman -S qt5`)
3. Install `libgit2` (e.g. `sudo pacman -S libgit2`)
4. Install `opencv` (e.g. `sudo pacman -S opencv`)
5. Download the CEF binary distribution and point `CEF_ROOT` to it
    1. `wget https://cef-builds.spotifycdn.com/cef_binary_3.3578.1861.g1992780_linux64.tar.bz2`
    2. `tar -xvf cef_binary_3.3578.1861.g1992780_linux64.tar.bz2`
    3. `export CEF_ROOT=$(pwd)/cef_binary_3.3578.1861.g1992780_linux64`
6. Clone the repository and enter the directory
7. Build the project
    1. `mkdir build && cd build`
    2. `cmake -DCMAKE_BUILD_TYPE=Release ..`
    3. `make`

## Building a Flatpak

We use these builds for testing Flatpak runtimes and to pre-build the binaries we later ship to Flathub.

1. Install the runtime and matching SDK: `flatpak install flathub org.kde.Platform//5.15-23.08 org.kde.Sdk//5.15-23.08`
2. Build the app and put it in a repository: `flatpak-builder --repo=flatpak/repo --force-clean flatpak/build-dir flatpak/io.botfather.Nightly.yaml`
3. Install and run the app for testing:
    1. `flatpak --user install ./flatpak/repo io.botfather.Nightly`
    2. `flatpak run io.botfather.Nightly`

## Building on Windows

1. Install [Visual Studio](https://visualstudio.microsoft.com/) including the `Desktop development with C++` option
2. Install [CMake](https://cmake.org/download/)
3. Install [Qt 5](https://www.qt.io/download)
4. Set up Chromium Embedded Framework (CEF)
    1. Download https://cef-builds.spotifycdn.com/cef_binary_3.3578.1861.g1992780_windows64.tar.bz2
    2. Extract the archive to a persistent location
    3. Set the `CEF_ROOT` environment variable to the extracted directory
5. Set up OpenCV 4
    1. Download and run the [OpenCV installer for Windows](https://opencv.org/releases/)
    2. Select the OpenCV directory and a build directory in CMake
    3. Configure and make sure `opencv_world` is selected
    4. Generate and open the project
    4. Build `ALL_BUILD` in the Release configuration
    6. Point the `OPENCV_ROOT` environment variable to the build directory
6. Set up libgit2
    1. Download and extract the [latest libgit2 release](https://github.com/libgit2/libgit2/releases)
    2. Select the libgit2 directory and a build directory in CMake
    3. Configure, generate, and open the project
    4. Build `ALL_BUILD` in the Release configuration
    5. Add the libgit2 source and `build/Release` directories to the `CMAKE_PREFIX_PATH` environment variable
7. Build Botfather
    1. Clone the `botfather` repository
    2. Select the `botfather` directory and a build directory in CMake
    3. Configure, generate, and open the project
    4. Build `ALL_BUILD` in the Release configuration
    5. Copy `git2.dll` and `opencv_world4xxx.dll` to the `build/bin/Release` directory

## VSCode Setup

1. Install the [C/C++ VSCode Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
2. Update the extension's IntelliSense configuration

    1. Open the Command Palette (`Ctrl+Shift+P`)
    2. Search for `>C/C++: Edit Configurations (UI)`
    3. Add third-party dependencies to the `Include path` section

        On Arch Linux, the following should be sufficient:

        ```txt
        ${workspaceFolder}/**
        /usr/include/qt/**
        /usr/include/opencv4/**
        /path/to/cef_binary_3.3578.1861.g1992780_linux64/**
        ```

        On Windows, something like this should work:

        ```txt
        ${workspaceFolder}/**
        C:\Qt\5.12.9\msvc2015_64\include\**
        C:\LIBGIT2\libgit2-1.8.2\include\**
        C:\OPENCV\4.10.0\build\include\**
        C:\CEF\cef_binary_3.3578.1861.g1992780_windows64\**
        ```
## Troubleshooting

### `LaunchProcess: failed to execvp: ./webhelper`

This means there's no `webhelper` binary next to the `botfather` binary.
Fix this problem by placing a `webhelper` binary next to the `botfather` binary matching the platform and build type.

### Browser is blank / nothing works after CEF Update

Rebuild the both `webhelper` and `botfather`.

### How to choose a new CEF version

Look up the latest supported [CEF Release Branches](https://bitbucket.org/chromiumembedded/cef/wiki/BranchesAndBuilding.md#markdown-header-current-release-branches-supported) and attempt to use it.
In case the version appears unstable, use the release [CefSharp](https://github.com/cefsharp/CefSharp/releases) is based on.

### Debugging the Flatpak

Add the following build options to the `io.botfather.Nightly.yaml` to prevent the builder from stipping debug symbols from the build.

```json
"build-options": {
	"no-debuginfo": true,
	"strip": false
}
```

Then start a shell in the sandbox using `flatpak-builder --run <build-dir> <manifest> sh`.
Now use `gdb`, `strace`, `ldd` and so on to debug.

More infos can be found in the [Flatpak documentation on debugging](http://docs.flatpak.org/en/latest/debugging.html).
