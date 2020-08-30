add_library(gear-utils INTERFACE)
target_sources(gear-utils INTERFACE include/gear/Util.h)
target_include_directories(gear-utils INTERFACE include)