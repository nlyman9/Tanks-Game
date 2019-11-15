add_library(GameLoop
  STATIC
  GameLoop.hpp
  GameLoop.cpp
)

target_link_libraries(GameLoop
  PUBLIC
  Render
  Object
  Tank
  Constants
  Sprite
  MapGenerator
  Projectile
  Networking
  Multiplayer
)

target_include_directories(GameLoop
  PUBLIC
  .
)
