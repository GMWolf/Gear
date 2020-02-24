add_executable(font_pack main.cpp)
target_link_libraries(font_pack PUBLIC bfg_lyra stb nlohmann_json::nlohmann_json yaml-cpp)
