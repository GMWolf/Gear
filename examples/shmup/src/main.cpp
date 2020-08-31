//
// Created by felix on 08/01/2020.
//
#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/SpriteBatch.h>
#include <gear/CollisionShape.h>
#include <gear/Shader.h>
#include <gear/View.h>
#include <glm/gtc/type_ptr.hpp>
#include <gear/ecs/ECS.h>
#include <gear/Transform.h>
#include <gear/Texture.h>
#include <gear/BitmapFont.h>
#include <gear/AssetManager.h>
#include <gear/DebugUI.h>
#include <gear/map/TileMap.h>
#include <gear/map/TilemapSystem.h>
#include <gear/RenderSystem.h>
#include <gear/PrimDraw.h>
#include <gear/Input.h>

#include "Collisions.h"

#include <gear/Job.h>
#include <iostream>

namespace gecs = gear::ecs;

struct Player {
    float moveSpeed = 4;
    gear::Sprite bulletSprite;
    gear::CollisionShape  bulletShape;
    float shootTimer = 0;
    float shootSideOffset = 2;
};

struct Enemy {
    int health = 3;
};

struct Bullet {
    glm::vec2 vel;
};

struct DestroyOnAnimationEnd {
};

struct Text {
    std::string text;
    const gear::assets::Font* font;
};

struct Lifetime {
    float time;
};

struct Velocity {
    glm::vec2 v;
};

static int score = 0;

static void createStage(gear::AssetRegistry& assets, gear::ecs::CommandBuffer& cmd, gear::TextureStore& texStore) {
    {
        auto spr = gear::createSprite(assets.getSprite("ship2"), texStore);

        Player player;
        player.bulletSprite = gear::createSprite(assets.getSprite("bullet_blue1"), texStore);
        player.bulletShape = *player.bulletSprite.mask;

        cmd.createEntity( spr,
                gear::Transform{{480 / 2, 32}},
                *spr.mask,
                player);
    }

    //view
    {
        cmd.createEntity(gear::View{{0,0}, {480, 720}});
    }

    {
        auto map = assets.getMap("map");
        gear::TileMap tileMap = std::get<gear::Map::TileLayer>((*map).layers.front().variant).tileMap;
        cmd.createEntity(gear::Transform{{0,0}},
                         gear::TilemapComponent{tileMap});
    }
}

static void movePlayer(const gear::InputState& input, gear::ecs::Registry& ecs, gear::ecs::CommandBuffer& cmd) {


    {
        auto chunks = ecs.query(gear::ecs::Query().all<Player, gear::Transform>());

        for (auto c : chunks) {
            auto chunk = gear::ecs::ChunkView<gecs::EntityRef, Player, gear::Transform>(*c);

            for (auto[entity, player, transform] : chunk) {

                if (input.keyDown(gear::KEYS::LEFT)) {
                    transform.pos.x -= player.moveSpeed;
                }
                if (input.keyDown(gear::KEYS::RIGHT)) {
                    transform.pos.x += player.moveSpeed;
                }
                if (input.keyDown(gear::KEYS::UP)) {
                    transform.pos.y += player.moveSpeed;
                }
                if (input.keyDown(gear::KEYS::DOWN)) {
                    transform.pos.y -= player.moveSpeed;
                }

                if (player.shootTimer > 0) player.shootTimer--;

                if (input.keyDown(gear::KEYS::SPACE) && player.shootTimer <= 0) {
                    cmd.createEntity(gear::Sprite(player.bulletSprite),
                                     player.bulletShape,
                                     gear::Transform{transform.pos + glm::vec2(player.shootSideOffset, 24)},
                                     Bullet{{0, 2}}
                    );
                    player.shootSideOffset *= -1;

                    player.shootTimer = 12;
                }
            }

        }
    }

    {
        auto chunks = ecs.query(gear::ecs::Query().all<gear::Transform, Bullet>());
        for (auto c : chunks) {
            auto chunk = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::Transform, Bullet>(*c);
            for (auto[entity, transform, bullet] : chunk) {
                transform.pos += bullet.vel;
                if (transform.pos.y > 720) {
                    cmd.destroyEntity(entity);
                }
            }
        }
    }

    {
        auto chunks = ecs.query(gear::ecs::Query().all<Velocity, gear::Transform>());
        for (auto c: chunks) {
            auto chunk = gear::ecs::ChunkView<Velocity, gear::Transform>(*c);
            for (auto[velocity, transform] : chunk) {
                transform.pos += velocity.v;
            }
        }
    }

    {
        auto chunks = ecs.query(gear::ecs::Query().all<Enemy, gear::Transform>());
        for (auto c: chunks) {
            auto chunk = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::Transform>(*c);
            for (auto[entity, transform] : chunk) {
                if (transform.pos.y < 32) {
                    cmd.destroyEntity(entity);
                }
            }
        }
    }
}

static void processCollisions(CollisionFilter& filter, gear::ecs::CommandBuffer& cmd, gear::AssetRegistry& assets, gear::TextureStore& texStore) {
    for (auto& pair : filter.collisionPairs) {
        auto[enemy, t] = pair.entity[0].get<Enemy, gear::Transform>();

        if (--enemy.health <= 0) {
            score += 100;
            cmd.destroyEntity(pair.entity[0]);
            cmd.createEntity(t, gear::ecs::CopyProvider{gear::createSprite(assets.getSprite("explosion_0"), texStore)}, DestroyOnAnimationEnd{});
            cmd.createEntity(gear::Transform{t.pos + glm::vec2(-25, 25)},
                             Text{"100", assets.getFont("default")},
                             Lifetime{1});
        }
        cmd.destroyEntity(pair.entity[1]);
    }
}

static void processLifetime(gear::ecs::Registry& ecs, gear::ecs::CommandBuffer& cmd) {
    auto chunks = ecs.query(gecs::Query().all<Lifetime>());
    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::EntityRef, Lifetime>(*c);
        for (auto[entity, lifetime] : chunk) {
            lifetime.time -= 1 / 60.f;
            if (lifetime.time <= 0) {
                cmd.destroyEntity(entity);
            }
        }
    }
}

static void processAnimation(gear::ecs::Registry& ecs, gear::ecs::CommandBuffer& cmd) {

    {
        auto chunks = ecs.query(gecs::Query().all<gear::Sprite>());

        for (auto c : chunks) {
            auto chunk = gecs::ChunkView<gear::Sprite>(*c);
            for (auto[sprite] : chunk) {
                sprite.imageIndex++;
                if (sprite.imageIndex >= sprite.texRegions.size()) {
                    sprite.imageIndex = 0;
                }
            }
        }
    }

    {
        auto chunks = ecs.query(gecs::Query().all<gear::Sprite, DestroyOnAnimationEnd>());
        for (auto c: chunks) {
            auto chunk = gecs::ChunkView<gecs::EntityRef, gear::Sprite>(*c);
            for (auto[entity, sprite] : chunk) {
                if (sprite.imageIndex == 0) {
                    cmd.destroyEntity(entity);
                }
            }
        }
    }
}

static void spawnEnemy(gecs::Prefab prefab, gear::AssetRegistry& assets, gear::ecs::CommandBuffer& cmd) {
    cmd.createEntity(prefab,
            gear::Transform{{480.0f * (rand()/(float)RAND_MAX), 720}},
            Velocity{{0, -1.5 - 0.5*(rand() / (float)RAND_MAX)}});
}

void render(gear::SpriteBatch& batch, gear::AssetRegistry& assets, gear::ecs::Registry& ecs, gear::TextureStore& texStore, gear::ShaderStore& shaderStore) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    gear::View view {{0,0}, {480, 720}};

    //tiles
    {
        auto shd = shaderStore.getShader(assets.getShader("textured"));
        gear::tilemapSystemRender(ecs, *shd, texStore);
    }

    {
        auto shd = shaderStore.getShader(assets.getShader("textured"));
        gear::renderSprites(ecs, batch, *shd, texStore);
    }

    {
        auto font = assets.getFont("default");
        auto shd = shaderStore.getShader(assets.getShader("font"));
        shd->use();
        glUniform1i(shd->uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        auto chunks = ecs.query(gecs::Query().all<Text, gear::Transform>());
        for(auto c : chunks){
            auto chunk = gecs::ChunkView<gear::Transform, Text>(*c);
            for (auto[transform, text] : chunk) {
                gear::renderText(text.text, text.font, transform.pos, batch, texStore);
            }
        };

        gear::renderText("Score: " + std::to_string(score), font, glm::vec2(20, 680), batch, texStore);

        batch.flush();
    }

}

void debugDraw(gear::PrimDraw& dd, gear::AssetRegistry& assets, gear::ecs::Registry& ecs, gear::ShaderStore& shaderStore) {
    auto shd = shaderStore.getShader(assets.getShader("prim"));
    gear::renderDebugShapes(ecs, dd, *shd);
}

gear::ecs::Prefab createEnemyPrefab(gear::ecs::Registry& reg, gear::AssetRegistry& assets, gear::ecs::CommandBuffer& cmd, gear::TextureStore& texStore) {
    auto sprite = gear::createSprite(assets.getSprite("ship1"), texStore);
    gecs::EntityRef e = cmd.createEntity(
            Enemy{},
            *sprite.mask,
            sprite);

    return gecs::Prefab{e};
};

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *_app) override {
        app = _app;
        batch.emplace(100);
        primDraw.emplace();
        assets.emplace();
        textureStore.emplace();
        shaderStore.emplace();

        assets->loadBundle("assets.bin");

        enemyPrefab = createEnemyPrefab(prefabs, *assets, cmd, *textureStore);
        gecs::executeCommandBuffer(cmd, world);

        createStage(*assets, cmd, *textureStore);

        enemyBulletFilter.entityA = gear::ecs::Query().all<Enemy>();
        enemyBulletFilter.entityB = gear::ecs::Query().all<Bullet>();

        gear::ui::initialize(app->window);

        gear::Scheduler scheduler;
        scheduler.addJob([]() {
            std::cout << "yo!";
        });
        scheduler.resume();

    }

    void update() override {

        if( app->getInputState().keyPressed(gear::KEYS::ESCAPE)) {
            app->close();
        }

        gear::tilemapSystemCreateSystemComponent(world, cmd);

        movePlayer(app->getInputState(), world, cmd);

        enemyBulletFilter.collisionPairs.clear();
        checkCollisions(world, enemyBulletFilter);

        gecs::executeCommandBuffer(cmd, world);
        processCollisions(enemyBulletFilter, cmd, *assets, *textureStore);
        render(*batch, *assets, world, *textureStore, *shaderStore);
        debugDraw(*primDraw, *assets, world, *shaderStore);
        processAnimation(world, cmd);
        processLifetime(world, cmd);
        gecs::executeCommandBuffer(cmd, world);

        if (--spawnTimer <= 0) {
            spawnEnemy(enemyPrefab, *assets, cmd);
            spawnTimer = 24;
        }

        gear::ui::begin();
        perf.frameTime = app->frameTime;
        gear::ui::perfWindow(perf);
        gear::ui::end();
    }

    void end() override {
        gear::ui::cleanup();
        batch.reset();
        primDraw.reset();
        assets.reset();
        textureStore.reset();
        shaderStore.reset();
    }

private:

    gear::ecs::Registry world;
    gear::ecs::Registry prefabs;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};

    gear::ecs::Prefab enemyPrefab;

    CollisionFilter enemyBulletFilter;

    std::optional<gear::AssetRegistry> assets;
    std::optional<gear::SpriteBatch> batch;
    std::optional<gear::PrimDraw> primDraw;
    std::optional<gear::TextureStore> textureStore;
    std::optional<gear::ShaderStore> shaderStore;

    int spawnTimer = 10;
    gear::ui::PerfData perf {};
    gear::Application* app {};
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
