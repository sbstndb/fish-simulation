This is a very simple SDL-IMGUI exemple. 
There are the

### Prerequisities
You need to have 
- CMake 3.12 or higher
- C++ compiler (tested with g++@13)
- SDL2 (Simple DirectMedia Layer)

### Compilation steps
1. Clone this repo
2. Update Submodules :
```bash 
git submodule update --init --recursive
```
3. Create the build directory :
```bash 
mkduir build
cd build
```
4. Run CMake and Make :
```bash 
cmake ..
make -j
```
5. Run the executable : 
```bash 
./sdl-imgui
```


