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
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  SDL2
  SDL2_image
)