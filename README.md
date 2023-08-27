# Leeds-RayTracing

A CPU raytracer based on Qt using Monte-Carlo Method

## Environment
- Winodws 11
- Qt 6.5.1 msvc2019_64
- CMake Building
- Clion 2023.2

Input your own Qt Path in the `CMakeLists.txt` in the line 9.

```cmake
    set(CMAKE_PREFIX_PATH "INPUT YOUR OWN PATH")
```

## Build and Run
- Only Support Release mode if using msvc2019_64
- Debug and other modes works in MinGW, but not make all tests.

```bash
mkdir build
cd build 
cmake ..
```