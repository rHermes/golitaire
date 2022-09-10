
add_executable(golitaire main.cpp App.cpp App.h LTK/Shader.cpp LTK/Shader.h LTK/Program.cpp LTK/Program.h)

set_target_properties(golitaire
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(golitaire PRIVATE fmt::fmt)
target_link_libraries(golitaire PRIVATE glad2)
target_link_libraries(golitaire PRIVATE glfw)
target_link_libraries(golitaire PRIVATE glm::glm)

add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data)
add_dependencies(golitaire copy_data)