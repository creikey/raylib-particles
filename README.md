# raylib-particles
Particles life made in Raylib

## Building
```
mkdir build
cd build
cmake ..
make
```

## Raylib
Raylib is checked into the source tree and compiled with cmake. When upgrading make sure to change the name in .gitignore to ignore the big folders for examples and games, and then change the name in CMakeLists.txt for the new version. 