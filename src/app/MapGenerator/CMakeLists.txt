add_library(MapGenerator
  STATIC
  MapGenerator.hpp
  MapGenerator.cpp
  Quads.cpp
  Quads.hpp
  Tileset.cpp
  Tileset.hpp
  Quadrant.cpp
  Quadrant.hpp
  QuadConstants.cpp
  QuadConstants.hpp
  Graph.cpp
  Graph.hpp
  Map.hpp
  Map.cpp
  Edge.cpp
  Edge.hpp
)

target_include_directories(MapGenerator
  PUBLIC
  .
)