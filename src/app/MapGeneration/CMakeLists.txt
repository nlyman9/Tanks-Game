add_library(MapGeneration
  STATIC
  mirror_map.hpp
  mirror_map.cpp
  line_map.hpp
  line_map.cpp
)

target_link_libraries(MapGeneration
  PRIVATE
  Render
)

target_include_directories(MapGeneration
  PUBLIC
  .
)