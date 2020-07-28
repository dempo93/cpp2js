# Cpp2js #
Cmake project to demonstrate the compilation of C++ sources to Webassembly + js

## Required tools ##
* Cmake: Simply download and install following the instructions [here](https://cmake.org/download/)
* Emscripten SDK:
just 
```
git clone git@github.com:emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
```

## Building the example ## 
* locate the `Emscripten.cmake` in your emsdk source folder  (should be `/path/to/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake`)
* create a build folder and cd into it
```
cd /path/to/cpp2js
mkdir build
cd build
```

### Compile to webassembly and js ###
Notice: the build will **not** export the main 
* build with 
```
cmake -DCMAKE_TOOLCHAIN_FILE=<path_to>/Emscripten.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build -- -j8
```
* serve `/path/to/cpp2js` with your favorite web server

### Native compilation (for debugging) ###
Notice: a main function is provided for ease of testing and debugging. A suitable cpp compiler (gcc, clang) is required
```
cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build -- -j8
```