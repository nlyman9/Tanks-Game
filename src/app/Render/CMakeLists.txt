add_library(Render
  STATIC
  Render.hpp
  Render.cpp
)

target_link_libraries(Render
  PRIVATE
  SDL2
  SDL2_image
  Constants
  Player
  Object
  Sprite
  Enemy
  MapGeneration
)

target_include_directories(Render
  PUBLIC
  .
)