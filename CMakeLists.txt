cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME DICOM3D)
project(${PROJECT_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
add_subdirectory(dependencies)

# Add the src directory as the root include directory
target_include_directories(${PROJECT_NAME} PRIVATE src)