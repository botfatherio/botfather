# Create an AppImage

This needs to be done after [building on linux}(building-on-linux.md).

- [Download linuxdeploqt](https://github.com/probonopd/linuxdeployqt/releases) and make it executable
- Place both the botfather and botfather_helper build outputs in an `appdir` directory
- `repoquery -l nss-softokn | grep '.so\|.chk' | xargs -i cp -R {} ./appdir/lib`
- `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:appdir/`
- `./linuxdeployqt-continuous-x86_64.AppImage appdir/botfather -appimage`

## Troubleshooting:

#### "ERROR: ldd outputLine: "libwidevinecdm.so => not found""

```
ERROR: ldd outputLine: "libwidevinecdm.so => not found"
ERROR: for binary: "/home/john/Downloads/appdir/libwidevinecdmadapter.so"
ERROR: Please ensure that all libraries can be found by ldd. Aborting.
```
Don't package `libwidevinecdmadapter.so`, we don't need it and it requires `libwidevinecdm.so` which we don't have.

#### `ERROR: Desktop file missing`

```
ERROR: Desktop file missing, creating a default one (you will probably want to edit it)
```

Copy `botfather.desktop` from `botfather/metadata` into the `appdir` dir.

#### `ERROR: Icon file missing`

```
ERROR: Icon file missing, creating a default one (you will probably want to edit it)
```

Copy `botfather.png` from `botfather/metadata` into the `appdir` dir.

#### `libsoftokn3.so No such file or directory`
```
[1022/144633:ERROR:nss_util.cc(740)] Error initializing NSS with a persistent database (sql:/home/me/.pki/nssdb): libsoftokn3.so: cannot open shared object file: No such file or directory
[1022/144633:ERROR:nss_util.cc(211)] Error initializing NSS without a persistent database: NSS error code: -5925
[1022/144633:FATAL:nss_util.cc(213)] nss_error=-5925, os_error=0
```

The issue and multiple solutions are desripbed here: https://github.com/probonopd/linuxdeployqt/issues/35
Running `cp -r /usr/lib/x86_64-linux-gnu/nss lib/` inside the `appdir` dir worked for me **on Ubuntu 17.04**.
**On Centos 7** I had to run `repoquery -l nss-softokn | grep '.so\|.chk' | xargs -i cp -R {} ./appdir/lib`.

#### `strip: supported targets:`

```
strip: supported targets: elf64-x86-64 elf32-i386 elf32-iamcu elf32-x86-64 a.out-i386-linux pei-i386 pei-x86-64 elf64-l1om elf64-k1om elf64-little elf64-big elf32-little elf32-big plugin srec symbolsrec verilog tekhex binary ihex
make: *** [BuildOutput/botfather] Error 123
```

This happens when there are no .so files in the BuildOutput dir. 