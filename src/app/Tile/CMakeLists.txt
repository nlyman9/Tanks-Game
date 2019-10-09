add_library(Tile
  STATIC
  Tile.hpp
  Tile.cpp
)

target_link_libraries(Tile
  PRIVATE
  Object
)

target_include_directories(Tile
  PUBLIC
  .
)
