add_library(Constants
  STATIC
  Constants.hpp
  Constants.cpp
)

target_include_directories(Constants
  PUBLIC
  .
)