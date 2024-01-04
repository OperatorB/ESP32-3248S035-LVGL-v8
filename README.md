| Supported Targets | ESP32 |
| ----------------- | ----- |

# Usage

- Create an empty ESP-IDF project with name "LVGL_lovyanGFX". (e.g Visual Studio Code)
- Copy the content of this repo to your project.
- Create a "components" directory.
- Clone LVGL V8.3 release to the components directory. `git clone --single-branch --branch release/v8.3 https://github.com/lvgl/lvgl.git`
- Clone LovyanGFX v1.x to the components directory. `git clone https://github.com/lovyan03/LovyanGFX.git`


## Quick test
To check that your hardware is working use esptool to flash the provided LVGL_lovyanGFX.bin file to your board.


## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

## About Build

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
