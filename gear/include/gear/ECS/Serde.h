//
// Created by felix on 11/06/2020.
//

#ifndef GEAR_SERDE_H
#define GEAR_SERDE_H

#include <memory>
#include "Config.h"

namespace gear::ecs::serde {


    namespace flexbuffers {
        class Builder;
        class Reference;
    }

    using SerializeFunction = void(*)(void*, flexbuffers::Builder& );
    using DeserializeFunction = void(*)(void*, const flexbuffers::Reference&);

    class SerdeRegistry {
        class Impl;
        std::unique_ptr<Impl> impl;

    public:

        SerdeRegistry();
        ~SerdeRegistry();

        void registerComponent(ComponentId compoId, std::string& name, SerializeFunction serializeFunction, DeserializeFunction deserializeFunction);
    };


}


#endif //GEAR_SERDE_H
