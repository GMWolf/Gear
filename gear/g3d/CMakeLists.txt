
set(GEAR_G3D_PUBLIC
        include/gear/g3d/g3d.h)

set(GEAR_G3D_HEADERS )

set(GEAR_G3D_SOURCES
        src/g3d.cpp)

add_library(gear-g3d ${GEAR_G3D_PUBLIC} ${GEAR_G3D_HEADERS} ${GEAR_G3D_SOURCES})

add_library(g3d-glad glad/src/glad.c glad/include/glad/glad.h glad/include/KHR/khrplatform.h)
target_include_directories(g3d-glad PUBLIC glad/include)

target_link_libraries(imgui_ogl3 PRIVATE g3d-glad)

target_link_libraries(gear-g3d
        PRIVATE imgui imgui_glfw imgui_ogl3 libzstd_static freetype)
target_link_libraries(gear-g3d
        PUBLIC gear-core gear-assets g3d-glad gear-gapi
        )
target_include_directories(gear-g3d
        PUBLIC include
        PRIVATE include/gear/g3d)
