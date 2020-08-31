
set(GEAR_CORE_HEADERS
        include/gear/Allocators.h
        include/gear/CollisionDetection.h
        include/gear/CollisionShape.h
        include/gear/CollisionShape.h
        include/gear/DI.h
        include/gear/Job.h
        include/gear/Transform.h
        include/gear/View.h
        )


set(GEAR_CORE_SOURCES
        src/Allocators.cpp
        src/CollisionDetection.cpp
        src/Job.cpp
        src/Transform.cpp
        src/View.cpp
        )

add_library(gear-core ${GEAR_CORE_HEADERS} ${GEAR_CORE_SOURCES})
target_include_directories(gear-core PUBLIC include fbs)

target_link_libraries(gear-core PUBLIC glm Boost::context)
target_link_libraries(gear-core PUBLIC gear-utils gear-ecs)