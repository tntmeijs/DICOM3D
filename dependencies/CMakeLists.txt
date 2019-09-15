# GLFW
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PRIVATE glfw)

# GL3W
add_library(gl3w STATIC gl3w/src/gl3w.c gl3w/include/GL/gl3w.h)
target_include_directories(gl3w PRIVATE gl3w/include)
target_link_libraries(${PROJECT_NAME} gl3w)
target_include_directories(${PROJECT_NAME} PRIVATE gl3w/include)

# Group the generated third-party projects
if(MSVC)
    # GLFW
    set_target_properties(glfw PROPERTIES FOLDER dependencies)

    # GL3W
    set_target_properties(gl3w PROPERTIES FOLDER dependencies)
endif(MSVC)