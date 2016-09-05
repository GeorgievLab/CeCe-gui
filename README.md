
# CeCe GUI application

CeCe is a platform for advanced simulation of cells and cell signalling in 2D.

This repository contains only GUI application, for whole project see [https://github.com/GeorgievLab/CeCe](https://github.com/GeorgievLab/CeCe)

## Building from source

Building from source code is done by CMake (see Requirements).

### Linux & OSX

```bash
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

### Windows
```batch
md build
cd build
cmake -G "Visual Studio 14 2015" ..
cmake --build .
```

### Requirements

* C++11 compiler - GCC, Clang, MSVC.
* [CMake](https://cmake.org) v3.1 or newer.
* Qt5

### Optional requirements

* [libpng](http://libpng.org/pub/png/libpng.html) - image capturing.
* [zlib](http://zlib.net) - required by libpng.
* [GLFW3](http://www.glfw.org) - Simulation visualization.
