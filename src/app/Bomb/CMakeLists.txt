add_library(Bomb
  STATIC
  Bomb.hpp
  Bomb.cpp
)

target_link_libraries(Bomb
  PRIVATE
  Object
)

target_include_directories(Bomb
  PUBLIC
  .
)
