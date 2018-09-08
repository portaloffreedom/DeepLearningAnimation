# AnimationTest

Animation Test project with GDNative C++

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

Clone this repository recursively:
``` bash
git clone --recursive https://github.com/portaloffreedom/DeepLearningAnimation.git
```

### Prerequisites

The dependencies of AnimationTest are:
```
godot >= 3.0.6
scons >= 3.0.1
eigen >= 3.3.5
```

### Installing

Compile godot-cpp:
```bash
$ cd src/godot-cpp
$ godot --gdnative-generate-json-api godot_api.json
$ scons platform=windows/linux/osx headers_dir=../godot_headers generate_bindings=yes target=release
```

Compile this project:
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ make install
```

## Run
Open the project and run the example. A GDSCript loads the _simple_ example library and connects a _Button_ with the `get_data` function.
