# BotFather

With botfather one can write crossplatform bots for Android, Desktop and Web (browser) applications in JavaScript.
For this purpose Botfather provides several JavaScript APIs featuring image matching and more.
Furthermore scripts can be turned into crossplatform binaries using the Botfather Binary Generator Tool (bingen for short).

## Documentation

- [Building a Flatpak](docs/building-a-flatpak.md)
- [Building on Windows](docs/building-on-windows-10.md)
- [Building on Linux](docs/building-on-linux.md)
- [Building an Appimage](docs/building-an-appimage.md)

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
