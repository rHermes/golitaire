
add_executable(golitaire main.cpp App.cpp App.h LTK/Shader.cpp LTK/Shader.h LTK/ShaderProgram.cpp LTK/ShaderProgram.h stb_image_implementation.cpp Game.cpp Game.h LTK/Texture2D.cpp LTK/Texture2D.h LTK/ResourceManager.cpp LTK/ResourceManager.h LTK/Misc.h stb_image_write_implementation.cpp glfwpp/Context.cpp glfwpp/Context.h glfwpp/Window.cpp glfwpp/Window.h glfwpp/WindowBuilder.cpp glfwpp/WindowBuilder.h SceneRenderer.cpp SceneRenderer.h CardEnums.h utils.cpp utils.h Card.cpp Card.h LTK/Vertex.cpp LTK/Vertex.h LTK/VAO.cpp LTK/VAO.h LTK/Buffer.cpp LTK/Buffer.h SceneObject.cpp SceneObject.h)

set_target_properties(golitaire
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if(MSVC)
    target_compile_options(golitaire PRIVATE /W4 /WX)
else()
    target_compile_options(golitaire PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
endif()

target_link_libraries(golitaire PRIVATE spdlog::spdlog)
target_link_libraries(golitaire PRIVATE fmt::fmt)
target_link_libraries(golitaire PRIVATE glad2)
target_link_libraries(golitaire PRIVATE glfw)
target_link_libraries(golitaire PRIVATE glm::glm)
target_link_libraries(golitaire PRIVATE stb)

add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data)
add_dependencies(golitaire copy_data)