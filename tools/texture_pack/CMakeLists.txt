add_executable(texture_pack main.cpp TexturePacker.cpp TexturePacker.h)
target_link_libraries(texture_pack PUBLIC bfg_lyra indica stb nlohmann_json::nlohmann_json)