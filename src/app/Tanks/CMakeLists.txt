add_library(Tank
  STATIC
  Tank.hpp
  Tank.cpp
  Player.hpp
  Player.cpp
  Enemy.hpp
  Enemy.cpp
)

if(${APPLE})
target_link_libraries(Tank
  PRIVATE
  Object
  Constants
  Sprite
  ${SDL2_LIBRARY}
)
else()
target_link_libraries(Tank
  PRIVATE
  Object
  Constants
  Sprite
)
endif()

target_include_directories(Tank
  PUBLIC
  .
)

