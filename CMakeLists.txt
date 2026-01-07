cmake_minimum_required(VERSION 3.13)

project(shell-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(shell
    src/main.cpp
    src/builtins.cpp
    src/utils.cpp
    src/executor.cpp
    src/parser.cpp
    )

target_compile_features(shell PRIVATE cxx_std_23)

target_compile_options(shell
     PRIVATE
        -Wall 
        -Wextra
        -Wpedantic
        -Werror)

target_include_directories(shell
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
