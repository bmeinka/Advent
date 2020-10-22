cmake_minimum_required(VERSION 3.0)
project(advent LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(core)
add_subdirectory(2015)
