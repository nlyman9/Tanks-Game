# Creating a new folder
1. Add CMakeLists.txt to directory
2. Add the following fields to CMakeLists.txt:
  * add_library(FOLDER_NAME
    STATIC
    ___.cpp
    ___.hpp
  )

  * target_include_directories(
    PUBLIC
    .
  )
  
  * target_link_libraries(
    PRIVATE
    EVERY_LIBRARY_YOU_NEED
  )
3. Open up CMakeLists.txt at the app level and add the folder to the list of subdirectories