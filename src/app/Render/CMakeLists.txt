add_library(Render
  STATIC
  Render.hpp
  Render.cpp
)

target_link_libraries(Render
  PRIVATE
  SDL2
  Constants
  Player
  Object
  Sprite
  Enemy
  Projectile
  ImageLoader
)

target_include_directories(Render
  PUBLIC
  .
)
