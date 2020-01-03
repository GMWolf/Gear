add_library(gear include/gear/Application.h src/Application.cpp include/gear/ApplicationAdapter.h include/gear/SpriteBatch.h)
target_link_libraries(gear PUBLIC glfw glad)
target_include_directories(gear PUBLIC include)