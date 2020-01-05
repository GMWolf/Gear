add_library(stb INTERFACE)
target_sources(stb INTERFACE deps/stb/stb_image.h)
target_include_directories(stb INTERFACE deps/stb)