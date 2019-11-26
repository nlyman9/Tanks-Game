add_library(Enemy
  STATIC
  Enemy.hpp
  Enemy.cpp
)

target_link_libraries(Enemy
  PRIVATE
  Object
  Player
  Sprite
  Constants
  Networking
  Projectile
)

target_include_directories(Enemy
  PUBLIC
  .
)
