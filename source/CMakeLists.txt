cmake_minimum_required (VERSION 3.10.2 FATAL_ERROR)
project(Game)

enable_language(C)
enable_language(CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(app)
