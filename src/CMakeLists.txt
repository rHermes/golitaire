
add_executable(golitaire main.cpp App.cpp App.h glfw_cpp/Context.cpp glfw_cpp/Context.h)

set_target_properties(golitaire
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(golitaire PRIVATE fmt::fmt)
target_link_libraries(golitaire PRIVATE glad2)
target_link_libraries(golitaire PRIVATE glfw)