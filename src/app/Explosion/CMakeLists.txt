add_library(Explosion
  STATIC
  Explosion.hpp
  Explosion.cpp
)

target_link_libraries(Explosion
  PRIVATE
  Object
)

target_include_directories(Explosion
  PUBLIC
  .
)
