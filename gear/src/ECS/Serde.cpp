//
// Created by felix on 11/06/2020.
//

#include <gear/ECS/Serde.h>
#include <unordered_map>

class gear::ecs::serde::SerdeRegistry::Impl {

    std::unordered_map<ComponentId,

};


void gear::ecs::serde::SerdeRegistry::registerComponent(gear::ecs::ComponentId compoId, std::string &name,
                                                        gear::ecs::serde::SerializeFunction serializeFunction,
                                                        gear::ecs::serde::DeserializeFunction deserializeFunction) {

}


