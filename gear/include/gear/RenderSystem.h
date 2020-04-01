//
// Created by felix on 01/04/2020.
//

#ifndef TBTG_RENDERSYSTEM_H
#define TBTG_RENDERSYSTEM_H

#include <gear/ECS/ECS.h>
#include <gear/SpriteBatch.h>
#include <gear/Shader.h>
#include <gear/View.h>
#include <gear/CoreComponents.h>

namespace gear {

    void render(gear::ecs::Registry &ecs, gear::SpriteBatch &batch, const gear::Shader &shader);

}
#endif //TBTG_RENDERSYSTEM_H
