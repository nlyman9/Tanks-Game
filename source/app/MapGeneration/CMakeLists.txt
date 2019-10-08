add_library(mirror
  STATIC
  mirror.hpp
  mirror.cpp
  lines_map.hpp
  lines_map.cpp
)

target_link_libraries(mirror
  PRIVATE
  Render
)

target_include_directories(mirror
  PUBLIC
  .
)