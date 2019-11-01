add_library(Credits
  STATIC
  Credits.hpp
  Credits.cpp
)

target_include_directories(Credits
  PUBLIC
  .
)

if(${APPLE})
target_link_libraries(Credits
  PRIVATE
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
)
else()
target_link_libraries(Credits
  PRIVATE
  SDL2
  SDL2_image
)
endif()