# Botfather

With Botfather one can write crossplatform bots for Android, Desktop and Web (browser) applications in JavaScript.
For this purpose Botfather provides several JavaScript APIs featuring image matching and more.
Furthermore scripts can be turned into crossplatform binaries using the Botfather Binary Generator Tool (bingen for short).

## Documentation

- [Building a Flatpak](docs/building-a-flatpak.md)
- [Building on Windows](docs/building-on-windows-10.md)
- [Building an Appimage](docs/building-an-appimage.md)

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
        ```

        On Windows, something like this should work:

        ```txt
        ${workspaceFolder}/**
        C:\Qt\5.12.9\msvc2015_64\include\**
        C:\LIBGIT2\libgit2-0.27.7\include\**
        C:\OPENCV\4.2.0\build\include\**
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
