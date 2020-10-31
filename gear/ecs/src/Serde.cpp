//
// Created by felix on 11/06/2020.
//

#include <Serde.h>
#include <unordered_map>
#include <string>


class gear::ecs::serde::SerdeRegistry::Impl {
public:
    std::unordered_map<ComponentId, SerializeFunction> serialize;
    std::unordered_map<ComponentId, DeserializeFunction> deserialize;
    std::unordered_map<ComponentId, std::string> name;

};


void gear::ecs::serde::SerdeRegistry::registerComponent(gear::ecs::ComponentId compId, std::string &name,
                                                        gear::ecs::serde::SerializeFunction serializeFunction,
                                                        gear::ecs::serde::DeserializeFunction deserializeFunction) {
    impl->serialize[compId] = serializeFunction;
    impl->deserialize[compId] = deserializeFunction;
    impl->name[compId] = name;
}
