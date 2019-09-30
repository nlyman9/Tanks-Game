add_library(Controllers
  STATIC
  AIController.cpp
  AIController.hpp
  KeyboardController.cpp
  KeyboardController.hpp
  NetworkController.cpp
  NetworkController.hpp
)

target_include_directories(Controllers
  PUBLIC
  .
)