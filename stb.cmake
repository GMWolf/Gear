add_library(stb stb_impl.cpp deps/stb/stb_image.h deps/stb/stb_rect_pack.h)
target_include_directories(stb PUBLIC deps/stb)