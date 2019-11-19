add_library(Player
  STATIC
  Player.hpp
  Player.cpp
)
if(${APPLE})
target_link_libraries(Player
  PRIVATE
  Object
  Constants
  Sprite
  ${SDL2_LIBRARY}
)
else()
target_link_libraries(Player
  PRIVATE
  Object
  Constants
  Sprite
)
endif()


target_include_directories(Player
  PUBLIC
  .
)
