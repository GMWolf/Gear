//
// Created by felix on 01/09/2020.
//

#include <Sprite.h>

static gear::CollisionShape getShape(const gear::assets::Shape type, const void* shape)
{
    if (shape) {
        switch (type) {
            case gear::assets::Shape::NONE:
                return std::monostate{};
            case gear::assets::Shape::rectangle: {
                auto rect = (gear::assets::Rectangle*) shape;
                return gear::Rectangle{
                        {rect->x(), rect->y()},
                        {rect->x() + rect->w(), rect->y() + rect->h()}
                };
            }
            case gear::assets::Shape::circle: {
                auto circle = (gear::assets::Circle*)shape;
                return gear::Circle{
                        {circle->x(), circle->y()},
                        circle->r()
                };
            }
            case gear::assets::Shape::point:
                break;
        }
    }

    return std::monostate{};
}

gear::CollisionShape gear::getObject(const gear::assets::Sprite *sprite, const std::string &name) {

    if (auto shape = sprite->objects()->LookupByKey(name.c_str())){
        return getShape(shape->shape_type(), shape->shape());
    }
    return std::monostate{};

}

gear::CollisionShape gear::getCollisionMask(const gear::assets::Sprite *sprite) {
    return getShape(sprite->collisionMask_type(), sprite->collisionMask());
}
