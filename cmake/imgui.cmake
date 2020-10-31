
add_library(imgui
        deps/imgui/imgui.h
        deps/imgui/imgui.cpp
        deps/imgui/imgui_widgets.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_demo.cpp)
target_include_directories(imgui PUBLIC deps/imgui)

add_library(imgui_glfw
        deps/imgui/examples/imgui_impl_glfw.h
        deps/imgui/examples/imgui_impl_glfw.cpp)
target_include_directories(imgui_glfw PUBLIC deps/imgui/examples)
target_link_libraries(imgui_glfw PUBLIC imgui)

add_library(imgui_ogl3
        deps/imgui/examples/imgui_impl_opengl3.h
        deps/imgui/examples/imgui_impl_opengl3.cpp)
target_include_directories(imgui_ogl3 PUBLIC deps/imgui/examples)
target_link_libraries(imgui_ogl3 PUBLIC imgui)
target_compile_definitions(imgui_ogl3 PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)