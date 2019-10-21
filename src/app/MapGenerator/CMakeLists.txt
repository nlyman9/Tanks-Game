add_library(MapGenerator
  STATIC
  MapGenerator.hpp
  MapGenerator.cpp
)

target_include_directories(MapGenerator
  PUBLIC
  .
)