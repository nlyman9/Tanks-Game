add_library(ImageLoader
  STATIC
  ImageLoader.hpp
  ImageLoader.cpp
)

target_include_directories(ImageLoader
  PUBLIC
  .
)

target_link_libraries(ImageLoader
  PRIVATE
  SDL2
  SDL2_image

)