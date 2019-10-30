add_library(Player
  STATIC
  Player.hpp
  Player.cpp
)

target_link_libraries(Player
  PRIVATE
  Object
  Constants
  Sprite
  SDL2
  Enemy
)

target_include_directories(Player
  PUBLIC
  .
)
