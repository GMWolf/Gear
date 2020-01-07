//
// Created by felix on 08/01/2020.
//

#include <gear/CollisionDetection.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <variant>

bool gear::collide(gear::CollisionShape a, glm::vec2 pa, gear::CollisionShape b, glm::vec2 pb) {
    return std::visit([pa,pb](auto&& a, auto&& b){return collide(a, pa, b, pb);}, a, b);
}


bool gear::collide(gear::Circle a, glm::vec2 pa, gear::Circle b, glm::vec2 pb) {
    float d2 = glm::distance2(a.center + pa, b.center + pb);
    float r = a.radius + b.radius;
    float r2 = r * r;
    return  d2 <= r2;
}

bool gear::collide(gear::Rectangle a, glm::vec2 pa, gear::Rectangle b, glm::vec2 pb) {
    return glm::all(glm::lessThan(a.min + pa, b.max + pb))
     && glm::all(glm::greaterThan(a.max + pa, b.min + pb));
}

bool gear::collide(gear::Circle a, glm::vec2 pa, gear::Rectangle b, glm::vec2 pb) {
    glm::vec2 n = glm::clamp(a.center + pa, b.min + pb, b.max + pb);
    return glm::distance2(n, a.center + pa) < a.radius * a.radius;
}

bool gear::collide(gear::Rectangle a, glm::vec2 pa, gear::Circle b, glm::vec2 pb) {
    return collide(b, pb, a, pa);
}
