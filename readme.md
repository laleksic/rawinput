# librawinput

## Description

librawinput is a cross-platform (Linux/Windows) wrapper over low-level raw input capabilities. Raw input allows getting input from different keyboards/mice and telling the difference between them-- a feature lacking in input systems such as SDL's input system. A use case would be a split screen game allowing each player to use his own mouse/keyboard pair.

You can see an older version of this library integrated with a split-screen fork of the Eternity Engine [here](https://github.com/laleksic/eternity/tree/rawinput). 

## Limitations

- On Linux requires user to have read access to `/dev/input/*` files.
- For now, only supports ANSI standard keyboard - full form factor and standard 3-button mice.
- No documentation yet, though a sample program is included.

## Compiling

Compile your program with `rawinput_common.c` and `rawinput_linux.c` or `rawinput_win32.c` depending on your platform. `rawinput.h` contains the public API.

The file `rawinput_demo.c` is an example program. You can compile it with:
`gcc rawinput_common.c rawinput_linux.c rawinput_demo.c -o rawinput_demo` if you're using `gcc` on Linux, or an equivalent command-line for other platforms.

## Usage

No documentation is yet available. For all usage information, read the included demo program.

