add_subdirectory(Credits)
add_subdirectory(Object)
add_subdirectory(Player)
add_subdirectory(GameLoop)
add_subdirectory(Networking)
add_subdirectory(Sprite)
add_subdirectory(Tile)
add_subdirectory(Projectile)
add_subdirectory(Render)
add_subdirectory(Enemy)
add_subdirectory(Constants)

add_executable(Game
  game.cpp
)

target_link_libraries(Game
  PRIVATE
  Credits
  GameLoop
)