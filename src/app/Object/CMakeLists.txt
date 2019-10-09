add_library(Object
  STATIC
  Object.hpp
  Object.cpp
)

target_link_libraries(Object
  PRIVATE
  Sprite
  SDL2
)

target_include_directories(Object
  PUBLIC
  .
)