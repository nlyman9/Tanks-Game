add_library(Sprite
  STATIC
  Sprite.hpp
  Sprite.cpp
)

target_include_directories(Sprite
  PUBLIC
  .
)

target_link_libraries(Sprite
  PUBLIC
  ImageLoader
)
