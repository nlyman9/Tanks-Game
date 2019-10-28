add_library(Networking
  STATIC
  Network.hpp
  Network.cpp
  Client.cpp
  Client.hpp
)

target_include_directories(Networking
  PUBLIC
  .
)

add_library(Server
  STATIC
  Server.cpp
  Server.hpp
)

target_include_directories(Server
  PUBLIC
  .
)

target_link_libraries(Server
  PRIVATE
  MapGenerator
)