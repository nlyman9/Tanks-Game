add_library(GameLoop
  STATIC
  GameLoop.hpp
  GameLoop.cpp
)

target_link_libraries(GameLoop
  PRIVATE
  Credits
  SDL2
  SDL2_image
)

target_include_directories(GameLoop
  PUBLIC
  .
)