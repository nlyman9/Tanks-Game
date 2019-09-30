add_subdirectory(Credits)
add_subdirectory(Object)
add_subdirectory(Player)
add_subdirectory(GameLoop)

link_directories(${PROJECT_SOURCE_DIR}/libraries/)

add_executable(Game
  game.cpp
)

target_link_libraries(Game
  PRIVATE
  Credits
  GameLoop
)