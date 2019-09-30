cmake_minimum_required (VERSION 3.10.2 FATAL_ERROR)
project(Game)

enable_language(C)
enable_language(CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(app)
link_directories(${PROJECT_SOURCE_DIR}/libraries/)