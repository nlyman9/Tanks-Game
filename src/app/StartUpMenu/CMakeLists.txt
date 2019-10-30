add_library(StartUpMenu
  STATIC
  StartUpMenu.cpp
  StartUpMenu.hpp
)

target_link_libraries(StartUpMenu
  PUBLIC
  GameLoop
  SDL2
  Render
)

target_include_directories(StartUpMenu
  PUBLIC
  .
)
