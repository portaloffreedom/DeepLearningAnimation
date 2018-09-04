# AnimationTest

Animation Test project with GDNative C++

# Build instructions
How to build your project. You don't have to reload the editor between builds.

First you need to download and build a couple of extra dependencies:

## Godot Headers
download godot headers in `src/godot_headers`
from `https://github.com/GodotNativeTools/godot_headers`

## Godot C++
download godot-cpp in `src/godot-cpp`
from `https://github.com/GodotNativeTools/godot-cpp`

Compile godot-cpp:
```bash
$ cd src/godot-cpp
$ godot --gdnative-generate-json-api godot_api.json
$ scons platform=windows/linux/osx headers_dir=../godot_headers generate_bindings=yes target=release
```

## CMake
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ make install
```

# Run
Open the project and run the example. A GDSCript loads the _simple_ example library and connects a _Button_ with the `get_data` function.
