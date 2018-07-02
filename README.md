# livdet-rest
REST API client

## How to compile
Dependencies needed to build:
* C++11 compiler
* OpenSSL

First, initialize the submodules:
```
$ git submodule update --recursive --init
```

Then:
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --config Release
```

The livdet-client binary will be in the build directory after building. 
