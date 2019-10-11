add_library(Object
  STATIC
  Object.hpp
  Object.cpp
)

target_include_directories(Object
  PUBLIC
  .
)
target_link_libraries(Object
  PUBLIC
  Constants
  Sprite
)