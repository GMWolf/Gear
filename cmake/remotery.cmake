add_library(remotery deps/Remotery/lib/Remotery.h deps/Remotery/lib/Remotery.c)
target_include_directories(remotery PUBLIC deps/Remotery/lib)
target_compile_definitions(remotery PUBLIC RMT_USE_OPENGL=1)
target_link_libraries(remotery PRIVATE pthread GL)