add_library(stb deps/stb_impl.cpp deps/stb/stb_image.h deps/stb/stb_rect_pack.h deps/stb/stb_truetype.h)
target_include_directories(stb PUBLIC deps/stb)