add_library(GameLoop
  STATIC
  GameLoop.hpp
  GameLoop.cpp
)

target_link_libraries(GameLoop
  PUBLIC
  Render
  Object
  Player
  Constants
  Enemy
  Sprite
)

target_include_directories(GameLoop
  PUBLIC
  .
)