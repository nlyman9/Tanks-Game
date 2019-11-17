add_subdirectory(Credits)
add_subdirectory(GameLoop)
add_subdirectory(Object)
add_subdirectory(Networking)
add_subdirectory(Sprite)
add_subdirectory(Projectile)
add_subdirectory(Render)
add_subdirectory(Tanks)
add_subdirectory(Constants)
add_subdirectory(StartUpMenu)
add_subdirectory(MapGenerator)
add_subdirectory(ImageLoader)
add_subdirectory(Args)

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
  Args
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
)

target_link_libraries(ServerProcess
  PRIVATE
  StartUpMenu
  Args
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
)
else()
target_link_libraries(Game
  PRIVATE
  StartUpMenu
  Args
)
target_link_libraries(ServerProcess
  PRIVATE
  Args
  StartUpMenu
)
endif()