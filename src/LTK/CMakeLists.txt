add_library(LTK
        Buffer.h
        EllipseRenderer2D.cpp EllipseRenderer2D.h
        Item3D.h Item3D.cpp
        Misc.h
        Plane.cpp Plane.h
        Ray.cpp Ray.h
        ResourceManager.cpp ResourceManager.h
        Shader.cpp Shader.h
        ShaderProgram.cpp ShaderProgram.h
        Texture2D.cpp Texture2D.h
        VAO.cpp VAO.h
        Vertex.cpp Vertex.h
        Ellipse2D.cpp Ellipse2D.h
        FlexiBuffer.h
        GraphicsRenderer2D.cpp GraphicsRenderer2D.h
        stb_image_implementation.cpp
        RenderItem2D.cpp RenderItem2D.h)

set_target_properties(LTK
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(LTK
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(LTK PUBLIC spdlog::spdlog)
target_link_libraries(LTK PUBLIC glad2)
target_link_libraries(LTK PUBLIC glm::glm)
target_link_libraries(LTK PUBLIC stb)