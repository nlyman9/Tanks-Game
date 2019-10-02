add_library(Projectile
  STATIC
  Projectile.hpp
  Projectile.cpp
)

target_link_libraries(Projectile
  PRIVATE
  Object
  Controllers
)

target_include_directories(Projectile
  PUBLIC
  .
)