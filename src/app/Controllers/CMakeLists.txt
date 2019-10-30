add_library(Controllers
    STATIC

    Controller.hpp
    Controller.cpp

    KeyboardController.hpp
    KeyboardController.cpp
)

target_include_directories(Controllers
    PUBLIC
    .
)