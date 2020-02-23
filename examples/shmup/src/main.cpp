//
// Created by felix on 08/01/2020.
//
#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/DI.h>
#include <gear/TextureAtlas.h>
#include <gear/SpriteBatch.h>
#include <gear/CollisionDetection.h>
#include <gear/CollisionShape.h>
#include <gear/Shader.h>
#include <gear/View.h>
#include <glm/gtc/type_ptr.hpp>
#include <gear/ECS/ECS.h>
#include <gear/CoreComponents.h>


#include "Collisions.h"

struct Player {
    float moveSpeed = 4;
    gear::Sprite bulletSprite;
    gear::CollisionShape  bulletShape;
    float shootTimer = 0;
};

struct Enemy {
    int health = 3;
};

struct Bullet {
    glm::vec2 vel;
};

struct DestroyOnAnimationEnd {
};

static void createStage(gear::TextureAtlas& atlas, gear::ecs::CommandBuffer& cmd) {


    {
        gear::Sprite spr = atlas.getSprite("ship1");
        for (int i = 0; i < 5; i++) {

            cmd.createEntity(spr,
                             gear::Transform{{480 * i / 5, 600}},
                             gear::CollisionShape{gear::Rectangle{{0,0}, {spr.size}}},
                             Enemy{}
                    );
        }
    }

    {
        gear::Sprite spr = atlas.getSprite("ship2");

        Player player;
        player.bulletSprite = atlas.getSprite("bullet_blue1");
        player.bulletShape = gear::Rectangle{{0,0}, player.bulletSprite.size};

        cmd.createEntity( spr,
                gear::Transform{{480 / 2, 32}},
                gear::CollisionShape{gear::Rectangle{{0,0}, {spr.size}}},
                player);

    }
}

static void movePlayer(gear::Application* app, gear::ecs::World& world, gear::ecs::CommandBuffer& cmd) {

    world.foreachChunk<Player, gear::Transform>(
            [&](auto chunk){
                for(auto [player, transform] : chunk) {
                    if (app->keyPressed(gear::KEYS::RIGHT)) {
                        transform.pos.x += player.moveSpeed;
                    }
                    if (app->keyPressed(gear::KEYS::LEFT)) {
                        transform.pos.x -= player.moveSpeed;
                    }
                    if (app->keyPressed(gear::KEYS::UP)) {
                        transform.pos.y += player.moveSpeed;
                    }
                    if (app->keyPressed(gear::KEYS::DOWN)) {
                        transform.pos.y -= player.moveSpeed;
                    }

                    if (player.shootTimer > 0) player.shootTimer--;

                    if (app->keyPressed(gear::KEYS::SPACE) && player.shootTimer <= 0) {

                        cmd.createEntity( player.bulletSprite,
                                player.bulletShape,
                                gear::Transform{transform.pos + glm::vec2(0, 24)},
                                Bullet{{0, 10}}
                                );

                        player.shootTimer = 12;
                    }
                }
            });

    world.foreachChunk<gear::ecs::Entity, gear::Transform, Bullet>(
            [&](auto chunk){
                for(auto [entity, transform, bullet] : chunk) {
                    transform.pos += bullet.vel;
                    if (transform.pos.y > 720) {
                        cmd.destroyEntity(entity);
                    }
                }
            });

    world.foreachChunk<gear::ecs::Entity, Enemy, gear::Transform>(
            [&](auto chunk) {
                for(auto [entity, enemy, transform] : chunk) {
                    if (transform.pos.y > 32) {
                        transform.pos.y -= 2;
                    } else {
                        cmd.destroyEntity(entity);
                    }
                }
            });
}

static void processCollisions(gear::ecs::World& world, gear::ecs::CommandBuffer& cmd, gear::TextureAtlas& atlas) {
    const gear::ecs::Archetype enemyArch = gear::ecs::Archetype::create<Enemy>();
    const gear::ecs::Archetype bulletArch = gear::ecs::Archetype::create<Bullet>();
    world.foreachChunk<gear::ecs::Entity, CollisionPair>(
            [&](auto chunk) {
                for(auto [collisionEntity, collision] : chunk) {
                    if (auto entities = collision.get(enemyArch, bulletArch)) {
                        auto [enemy, t] = world.get<Enemy, gear::Transform>(entities->first);

                        if (--enemy.health <= 0) {
                            cmd.destroyEntity(entities->first);
                            cmd.createEntity( t,  atlas.getSprite("explosion"), DestroyOnAnimationEnd{});
                        }
                        cmd.destroyEntity(entities->second);
                    }
                    cmd.destroyEntity(collisionEntity);
                }
            });
}


static void processAnimation(gear::ecs::World& world, gear::ecs::CommandBuffer& cmd) {
    world.foreachChunk<gear::Sprite>(
            [&](auto chunk) {
                for(auto [sprite] : chunk) {
                    sprite.imageIndex++;
                    if (sprite.imageIndex >= sprite.texRegions.size()) {
                        sprite.imageIndex = 0;
                    }
                }
            }
            );

    world.foreachChunk<gear::ecs::Entity, gear::Sprite, DestroyOnAnimationEnd>(
            [&](auto chunk) {
                for(auto [entity, sprite, d] : chunk) {
                    if (sprite.imageIndex == 0) {
                        cmd.destroyEntity(entity);
                    }
                }
            }
            );
}

static void spawnEnemy(gear::TextureAtlas& atlas, gear::ecs::CommandBuffer& cmd) {
    gear::Sprite spr = atlas.getSprite("ship1");
    cmd.createEntity(spr,
                     gear::Transform{{480.0f * (rand()/(float)RAND_MAX), 720+spr.size.y}},
                     gear::CollisionShape{gear::Rectangle{{0,0}, {spr.size}}},
                     Enemy{}
    );

}

void render(gear::SpriteBatch& batch, gear::Shader& shd, gear::ecs::World& ecsWorld) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    gear::View view {{0,0}, {480, 720}};

    shd.use();
    glUniform1i(shd.uniformLocation("tex"), 0);
    auto vm = view.matrix();
    glUniformMatrix4fv(shd.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

    ecsWorld.foreachChunk<gear::Sprite, gear::Transform>(
            [&](auto chunk){
                for(auto [sprite, transform] : chunk) {
                    batch.draw(sprite, transform.pos);
                }
            });

    batch.flush();
}

static void executeCommandBuffer(gear::ecs::World& world, gear::ecs::CommandBuffer& cmd) {
    world.executeCommandBuffer(cmd);
    cmd.commands.clear();
}

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *app) override {
        di.emplace<gear::ecs::World>();
        di.emplace<gear::TextureAtlas>("shmup_textures.json");
        di.emplace<gear::SpriteBatch>(500);
        di.emplace<gear::Shader>("simple_textured");
        di.emplace<gear::Application*>(app);
        di.emplace<gear::ecs::CommandBuffer>();

        di.invoke(createStage);
    }

    void update() override {
        di.invoke(movePlayer);
        di.invoke(checkCollisions);
        di.invoke(executeCommandBuffer);
        di.invoke(processCollisions);
        di.invoke(render);
        di.invoke(processAnimation);
        di.invoke(executeCommandBuffer);
        if (--spawnTimer <= 0) {
            di.invoke(spawnEnemy);
            spawnTimer = 60;
        }
    }

    void end() override {
        di.reset();
    }

private:
    gear::DI di;
    int spawnTimer = 10;
};

int main() {

    gear::AppConfig config {
        480, 720,
        "shmup",
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}