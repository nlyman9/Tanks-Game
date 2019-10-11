add_library(MapGeneration
  STATIC
  MapGenerator.hpp
  MapGenerator.cpp
)

target_include_directories(MapGeneration
  PUBLIC
  .
)