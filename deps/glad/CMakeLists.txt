add_library(glad src/glad.cpp include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(glad PUBLIC include)