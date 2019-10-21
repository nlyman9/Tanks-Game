add_library(Networking
  STATIC
  Network.hpp
  Network.cpp
)

target_include_directories(Networking
  PUBLIC
  .
)