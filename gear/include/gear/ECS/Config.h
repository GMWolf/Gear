//
// Created by felix on 08/02/2020.
//

#ifndef GEAR_CONFIG_H
#define GEAR_CONFIG_H

#include <cstdint>

namespace gear::ecs {
    using EntityId = uint32_t;
    using ChunkId = uint32_t;
    using ComponentId = uint32_t;

#ifdef GEAR_ECS_CHUNK_SIZE
    const uint16_t ChunkSize = GEAR_ECS_CHUNK_SIZE;
#else
    const uint16_t ChunkSize = 1024;
#endif

#ifdef GEAR_ECS_MAX_COMPONENTS
    const uint16_t MaxComponents = GEAR_ECS_MAX_COMPONENTS;
#else
    const uint16_t MaxComponents = 64;
#endif

}

#endif //GEAR_CONFIG_H
