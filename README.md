# livdet-rest
REST API client

## How to compile
Dependencies needed to build:
* cmake
* C++11 compiler
* OpenSSL (optional)

First, initialize the submodules:
```
$ git submodule update --recursive --init
```

### Compiling on Linux
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --config Release
```

The livdet-client binary will be in the build directory after building. 

### Compiling on Windows (MSVC)
Open the command prompt (CMD) and enable the MSVC environment, to do this you need to execute the vcvars64.bat batch script. You can locate where the vcvars64.bat file is using the ```dir``` command:
```
> cd "C:\Program Files (x86)\Microsoft Visual Studio"
> dir /b/s vcvars64.bat
```

Once you have the full path of the vcvars64.bat file, execute it:
```
> call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
```

Then enter to powershell:
```
> powershell
> mkdir build
> cd build
> cmake -DBUILD_CPR_TESTS=OFF -DCMAKE_USE_OPENSSL=OFF -G "Visual Studio 15 2017 Win64" ..
> cmake --build . --config Release
```

The "Visual Studio 15 2017 Win64" string might change depending on what version of Visual Studio your system has installed.

After the build is complete, navigate to the 'Release' directory inside the 'build' directory, copy the libcurl.dll file to it, then you can execute the livdet-client.exe program:
```
> cd Release
> cp ..\lib\Release\libcurl.dll .
> .\livdet-client.exe --help
```
