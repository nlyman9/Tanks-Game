add_library(Networking
  STATIC
  Network.hpp
  Network.cpp
  Server.cpp
  Client.cpp
)

target_include_directories(Networking
  PUBLIC
  .
)