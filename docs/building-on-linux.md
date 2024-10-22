# Build instructions for Arch Linux or Ubuntu 18.04

### Step 1: Install tools and some dependencies

- On Ubuntu 18.04: `sudo apt install git qtdeclarative5-dev qtmultimedia5-dev qtscript5-dev libqt5x11extras5-dev`
- On Arch Linux: `sudo pacman -S xlib`
- On Centos 7: `sudo yum install git cmake libXScrnSaver qt5-qtdeclarative-devel qt5-qtscript-devel qt5-qtmultimedia-devel qt5-qtx11extras-devel`

On Centos 7 `libXScrnSaver` is installed as workaround for the following error described [here](https://github.com/atom/atom/issues/13176)

```error while loading shared libraries: libXss.so.1: cannot open shared object file: No such file or directory```

#### (Optional) Install QtCreator

- On Ubuntu 18.04: `sudo apt install qtcreator`
- On Arch Linux: `sudo pacman -S qtcreator`
- On Centos 7: `sudo yum install https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm && sudo yum install qt-creator`

### Step 2: Build Chromium Embedded Framework

- Change directory to botfather/3rdparty/CEF
- Run ```chmod +x CEF_setup.sh```
- Run ```./CEF_setup.sh```
- Follow the scripts instructions

### Step 3: Build OpenCV 3

- `wget https://github.com/opencv/opencv/archive/3.4.3.zip`
- `unzip 3.4.3.zip`
- `cd opencv-3.4.3`
- `mkdir build && cd build`
- `cmake -D CMAKE_BUILD_TYPE=Release ..`
- `make -j $nproc` (`nproc` returns the number of CPU cores you have)
- `export OPENCV_ROOT=/path/to/opencv/build` (put this in ~/.bashrc or ~/.xsession)

### Step 4: Build Libgit2

- `wget https://github.com/libgit2/libgit2/archive/v0.27.7.zip`
- `unzip v0.27.7.zip`
- `cd libgit2-v0.27.7`
- `mkdir build && cd build`
- `cmake -D CMAKE_BUILD_TYPE=Release -D BUILD_CLAR=OFF -D THREADSAFE=ON ..`
- `cmake --build .`
- `sudo make install`
- `export LIBGIT2_ROOT=/path/to/libgit2/build` (put this in ~/.bashrc or ~/.xsession)

### Step 5: Build webhelper and botfather

- `mkdir package-build`
- `qmake-qt5 "CONFIG+=release" ../package.pro`
- `make -j $nproc` (`nproc` returns the number of CPU cores you have)
- `make install` (creates a `package` directory and puts `botfather` and `webhelper` in it)

*or use qtcreator...*
