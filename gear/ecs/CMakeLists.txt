
set(GEAR_ECS_HEADERS
        include/gear/ecs/Archetype.h
        include/gear/ecs/ChunkView.h
        include/gear/ecs/CommandBuffer.h
        include/gear/ecs/Component.h
        include/gear/ecs/Config.h
        include/gear/ecs/Core.h
        include/gear/ecs/ECS.h
        include/gear/ecs/System.h
        include/gear/ecs/Serde.h
        )

set(GEAR_ECS_SOURCES
        src/Archetype.cpp
        src/CommandBuffer.cpp
        src/Component.cpp
        src/Core.cpp
        src/Serde.cpp
        )


add_library(gear-ecs ${GEAR_ECS_HEADERS} ${GEAR_ECS_SOURCES})
target_link_libraries(gear-ecs PUBLIC gear-utils)
target_include_directories(gear-ecs
        PRIVATE include/gear/ecs
        PUBLIC include)