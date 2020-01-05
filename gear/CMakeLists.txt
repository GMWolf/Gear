add_library(gear include/gear/Application.h src/Application.cpp include/gear/ApplicationAdapter.h include/gear/SpriteBatch.h src/SpriteBatch.cpp src/stb_impl.cpp include/gear/Texture.h src/Texture.cpp)
target_link_libraries(gear PUBLIC glfw glad stb glm)
target_include_directories(gear PUBLIC include)