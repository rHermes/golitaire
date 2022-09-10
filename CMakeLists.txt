cmake_minimum_required(VERSION 3.22)
project(golitaire)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_subdirectory("extern/fmt")
add_subdirectory("extern/glad2")
add_subdirectory("extern/glfw")
add_subdirectory("extern/glm")
add_subdirectory("extern/stb")

add_subdirectory("src/")

