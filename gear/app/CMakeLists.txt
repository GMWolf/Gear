
set(GEAR_APP_HEADERS
        include/gear/Application.h
        include/gear/ApplicationAdapter.h
        include/gear/Input.h
        include/gear/KeyboardKeys.h
        )

set(GEAR_APP_SOURCES
        src/Application.cpp
        src/Input.cpp)


add_library(gear-app ${GEAR_APP_HEADERS} ${GEAR_APP_SOURCES})
target_include_directories(gear-app
        PUBLIC include
        PRIVATE include/gear
)

target_link_libraries(gear-app
        PRIVATE glad glfw
        PUBLIC glm)