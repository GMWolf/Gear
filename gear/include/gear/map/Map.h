//
// Created by felix on 07/05/2020.
//

#ifndef GEAR_MAP_H
#define GEAR_MAP_H

#include <vector>
#include "TileMap.h"
#include <variant>
#include <string>



namespace gear {

    class Map {

    public:
        class Object {

        };

        struct TileLayer {
            TileMap tileMap;
        };

        struct ObjectLayer {
            std::vector<Object> objects;
        };

        struct Layer {
            std::string name;
            std::variant<std::monostate, TileLayer, ObjectLayer> variant;
        };

        std::vector<Layer> layers;
    };


    namespace assets {
        class Map;
    }
    class AssetRegistry;

    Map loadMap(const assets::Map* map, AssetRegistry& registry);
}




#endif //GEAR_MAP_H
