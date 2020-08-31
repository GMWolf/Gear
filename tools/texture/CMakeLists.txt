add_library(gear_tools_texture texture.h texture.cpp)
target_include_directories(gear_tools_texture PUBLIC .)
target_link_libraries(gear_tools_texture
        PRIVATE libzstd_static
        PUBLIC gear-assets-fbs)