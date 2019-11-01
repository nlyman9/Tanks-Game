add_library(Controllers
    STATIC

    Controller.hpp
    Controller.cpp

    KeyboardController.hpp
    KeyboardController.cpp

    NetworkController.hpp
    NetworkController.cpp
)

target_include_directories(Controllers
    PUBLIC
    .
)

if(${APPLE})
target_link_libraries(Controllers
  PUBLIC
  ${SDL2_LIBRARY}
)
else()
endif()