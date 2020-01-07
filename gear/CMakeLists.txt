add_library(gear
        include/gear/Application.h
        src/Application.cpp
        include/gear/ApplicationAdapter.h
        include/gear/SpriteBatch.h
        src/SpriteBatch.cpp
        include/gear/Texture.h
        src/Texture.cpp
        include/gear/Shader.h
        src/Shader.cpp
        include/gear/TextureAtlas.h
        src/TextureAtlas.cpp
        include/gear/CollisionShape.h
        include/gear/CollisionDetection.h
        src/CollisionDetection.cpp)
target_link_libraries(gear PUBLIC glfw glad stb glm nlohmann_json::nlohmann_json)
target_include_directories(gear PUBLIC include)