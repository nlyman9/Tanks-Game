add_library(mirror
  STATIC
  mirror_map.hpp
  mirror_map.cpp
  line_map.hpp
  line_map.cpp
)

target_link_libraries(mirror
  PRIVATE
  Render
)

target_include_directories(mirror
  PUBLIC
  .
)