add_library(Render
  STATIC
  Render.hpp
  Render.cpp
)

target_link_libraries(Render
  PRIVATE
  Credits
  SDL2
  SDL2_image
  GameLoop
  Constants
  Player
)

target_include_directories(GameLoop
  PUBLIC
  .
)