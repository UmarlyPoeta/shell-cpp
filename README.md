# shell-cpp

A simple Unix shell implementation written in C++.

## Description

This project is a custom shell built from scratch using C++. It provides a command-line interface that supports executing external programs and several built-in commands.

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./shell
```

After launching, you'll see a `$` prompt where you can enter commands.

## Features

### Built-in Commands

| Command | Description |
|---------|-------------|
| `echo`  | Print arguments to standard output |
| `cd`    | Change current directory (supports `~` for home) |
| `pwd`   | Print current working directory |
| `type`  | Display information about command type |
| `exit`  | Exit the shell |

### Other Features

- External command execution via `fork`/`execvp`
- PATH environment variable lookup
- Quoting support (single and double quotes)
- Escape character handling (`\`)

## Requirements

- C++23
- CMake 3.13+
- A compatible C++ compiler (GCC, Clang)
- GNU Readline library

## License

MIT License