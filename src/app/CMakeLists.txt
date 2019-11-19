add_subdirectory(Credits)
add_subdirectory(GameLoop)
add_subdirectory(Object)
add_subdirectory(Multiplayer)
add_subdirectory(Networking)
add_subdirectory(Sprite)
add_subdirectory(Projectile)
add_subdirectory(Tanks)
add_subdirectory(Constants)
add_subdirectory(StartUpMenu)
add_subdirectory(MultiplayerMenu)
add_subdirectory(MapGenerator)
add_subdirectory(Render)
add_subdirectory(ImageLoader)
add_subdirectory(Args)
add_subdirectory(Bomb)
add_subdirectory(Box)

if(${APPLE})
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
${SDL2_IMAGE_INCLUDE_DIR}
${SDL2_TTF_INCLUDE_DIR})
endif()

add_executable(Game
  game.cpp
)
add_executable(ServerProcess
  Networking/Server.cpp
)
if(${APPLE})
target_link_libraries(Game
  PRIVATE
  StartUpMenu
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  Multiplayer
)

target_link_libraries(ServerProcess
  PRIVATE
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  StartUpMenu
  Args
  Multiplayer
)
else()
target_link_libraries(Game
  PRIVATE
  StartUpMenu
  Multiplayer
  Args
)
target_link_libraries(ServerProcess
  PRIVATE
  StartUpMenu
)
endif()
