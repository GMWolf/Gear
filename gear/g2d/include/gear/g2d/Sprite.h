//
// Created by felix on 01/09/2020.
//

#ifndef GEAR_SPRITE_H
#define GEAR_SPRITE_H

#include "g2d.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <gear/sprite_generated.h>
#include <gear/CollisionShape.h>


namespace gear {
    CollisionShape getObject( const assets::Sprite* sprite, const std::string& name );
    CollisionShape getCollisionMask( const assets::Sprite* sprite );

}

#endif //GEAR_SPRITE_H
