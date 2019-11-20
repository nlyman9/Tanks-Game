add_library(GameLoop
  STATIC
  LocalGameLoop.hpp
  LocalGameLoop.cpp
  OnlineGameLoop.hpp
  OnlineGameLoop.cpp
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
  Args
)

target_include_directories(GameLoop
  PUBLIC
  .
)
