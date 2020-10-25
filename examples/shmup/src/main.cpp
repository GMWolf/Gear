//
// Created by felix on 08/01/2020.
//
#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/CollisionShape.h>
#include <gear/View.h>
#include <gear/ecs/ECS.h>
#include <gear/Transform.h>
#include <gear/g2d/Text.h>
#include <gear/Assets.h>
#include <gear/g2d/DebugUI.h>
#include <gear/g2d/TilemapSystem.h>
#include <gear/g2d/RenderSystem.h>
#include <gear/Input.h>
#include "Collisions.h"
#include <iostream>
#include <gear/g2d/g2d.h>
#include <gear/g2d/Sprite.h>

namespace gecs = gear::ecs;

struct Player {
    float moveSpeed = 4;
    const gear::assets::Sprite* bulletSprite;
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

struct Lifetime {
    float time;
};

struct Velocity {
    glm::vec2 v;
};

static int score = 0;

static void createStage(gear::AssetRegistry& assets, gear::ecs::CommandBuffer& cmd) {
    {
        auto spr = assets.getSprite("ship2");

        Player player;
        player.bulletSprite = assets.getSprite("bullet_blue1");
        player.bulletShape = gear::getCollisionMask( player.bulletSprite );

        cmd.createEntity( gear::SpriteComponent{spr},
                gear::Transform{{480 / 2, 32}},
                gear::getCollisionMask(spr),
                player);
    }

    //view
    {
        cmd.createEntity(gear::View{{0,0}, {480, 720}});
    }

    {
        auto map = assets.getMap("map");

        cmd.createEntity(gear::Transform{{0,0}},
                         gear::TilemapComponent{
                                 (gear::assets::TileSet*)map->layers()->Get(0)->tileSet()->ptr(),
                                 map->layers()->Get(0)->width(), map->layers()->Get(0)->height(),
                                 map->layers()->Get(0)->data()->data()
        });
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
                    cmd.createEntity(gear::SpriteComponent{player.bulletSprite},
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

static void processCollisions(CollisionFilter& filter, gear::ecs::CommandBuffer& cmd, gear::AssetRegistry& assets) {
    for (auto& pair : filter.collisionPairs) {
        auto[enemy, t] = pair.entity[0].get<Enemy, gear::Transform>();

        if (--enemy.health <= 0) {
            score += 100;
            cmd.destroyEntity(pair.entity[0]);
            cmd.createEntity(gear::Transform{t}, gear::SpriteComponent{assets.getSprite("explosion_0")}, gear::DestroyOnAnimationEnd{});
            cmd.createEntity(gear::Transform{t.pos + glm::vec2(-25, 25)},
                             gear::Text{"100", assets.getBitmapFont("default")},
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

static void spawnEnemy(gecs::EntityRef prefab, gear::AssetRegistry& assets, gear::ecs::CommandBuffer& cmd) {
    cmd.createEntityPrefab(prefab,
            gear::Transform{{480.0f * (rand()/(float)RAND_MAX), 720}},
            Velocity{{0, -1.5 - 0.5*(rand() / (float)RAND_MAX)}});
}

void render(gear::G2DInstance* g2d, gear::AssetRegistry& assets, gear::ecs::Registry& ecs, gear::ecs::CommandBuffer& cmd) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, 480, 720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    gear::View view {{0,0}, {480, 720}};

    {
        gear::tilemapSystemRender(g2d, ecs, assets.getShader("textured"));
    }

    {
        gear::renderSprites(g2d, ecs, assets.getShader("textured"), cmd);
    }

    {
        auto shd = assets.getShader("font");
        gear::renderTextSystem(g2d, ecs, shd);
        auto font = assets.getBitmapFont("default");
        gear::renderText(g2d, "Score: " + std::to_string(score), font, glm::vec2(20, 680), assets.getShader("font"), view);
    }

    gear::testTex(g2d);

    g2d->flush();
}

void debugDraw(gear::G2DInstance* g2d, gear::AssetRegistry& assets, gear::ecs::Registry& ecs) {
    gear::renderDebugShapes(g2d, ecs, assets.getShader("prim"));
}

gear::ecs::EntityRef createEnemyPrefab(gear::ecs::Registry& reg, gear::AssetRegistry& assets, gear::ecs::CommandBuffer& cmd) {
    auto sprite = gear::SpriteComponent{assets.getSprite("ship1")};
    gecs::EntityRef e = cmd.createEntity(
            Enemy{},
            gear::getCollisionMask(sprite.sprite),
            sprite);

    return e;
};

class Game : public gear::ApplicationAdapter {
public:

    void initG2d() {
        gear::SpriteBatchCreateInfo spriteBatchInfo = {
                .batchSize = 100
        };

        gear::G2DInstanceCreateInfo instanceInfo = {
                .spriteBatchCreateInfo = &spriteBatchInfo
        };

        g2d = new gear::G2DInstance(instanceInfo);
    }

    void init(gear::Application *_app) override {
        app = _app;

        initG2d();

        assets.emplace();
        assets->loadBundle("assets.bin");

        enemyPrefab = createEnemyPrefab(prefabs, *assets, cmd);
        gecs::executeCommandBuffer(cmd, world);

        createStage(*assets, cmd);

        enemyBulletFilter.entityA = gear::ecs::Query().all<Enemy>();
        enemyBulletFilter.entityB = gear::ecs::Query().all<Bullet>();

        gear::ui::initialize(app->window);

        gear::fontCacheAddChar(g2d, assets->getFont("6809 chargen"), 'N');
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
        processCollisions(enemyBulletFilter, cmd, *assets);
        render(g2d, *assets, world, cmd);
        debugDraw(g2d, *assets, world);
        processLifetime(world, cmd);
        if (--spawnTimer <= 0) {
            spawnEnemy(enemyPrefab, *assets, cmd);
            spawnTimer = 24;
        }

        gecs::executeCommandBuffer(cmd, world);

        gear::ui::begin();
        perf.frameTime = app->frameTime;
        gear::ui::perfWindow(perf);
        gear::ui::end();
    }

    void end() override {
        gear::ui::cleanup();
        delete g2d;
        assets.reset();
    }

private:

    gear::ecs::Registry world;
    gear::ecs::Registry prefabs;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};

    gear::ecs::EntityRef enemyPrefab;

    gear::G2DInstance* g2d;

    CollisionFilter enemyBulletFilter;

    std::optional<gear::AssetRegistry> assets;

    int spawnTimer = 10;
    gear::ui::PerfData perf {};
    gear::Application* app {};
};

int main() {

    gear::AppConfig config {
        .width = 480,
        .height = 720,
        .title ="shmup",
        .gapi = gear::g2dGetGapi()
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}
