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
#include <gear/ECS/ECS.h>
#include <gear/CoreComponents.h>
#include <gear/Texture.h>
#include <gear/BitmapFont.h>
#include <gear/AssetManager.h>
#include <gear/DebugUI.h>
#include <gear/map/TileMap.h>
#include <gear/map/TilemapSystem.h>
#include <gear/RenderSystem.h>
#include <gear/PrimDraw.h>

#include "Collisions.h"

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
    gear::AssetReference<gear::BitmapFont> font;
};

struct Lifetime {
    float time;
};

struct Velocity {
    glm::vec2 v;
};

static int score = 0;

static void createStage(gear::AssetRegistry& assets, gear::ecs::CommandEncoder& cmd) {
    {
        gear::Sprite spr = *assets.getSprite("ship2");

        Player player;
        player.bulletSprite = *assets.getSprite("bullet_blue1");
        player.bulletShape = *player.bulletSprite.mask;

        cmd.createEntity( spr,
                gear::Transform{{480 / 2, 32}},
                *spr.mask,
                player);

    }

    //collision filters
    {
        cmd.createEntity(CollisionFilter{
            gear::ecs::Query().all<Enemy>(),
            gear::ecs::Query().all<Bullet>()
        });
    }

    //view
    {
        cmd.createEntity(gear::View{{0,0}, {480, 720}});
    }
}

static void movePlayer(gear::Application* app, gear::ecs::Registry& ecs, gear::ecs::CommandEncoder& cmd) {

    const size_t chunkArraySize = 256;
    gear::ecs::Chunk* chunkArray[chunkArraySize];
    auto chunks = ecs.queryChunks(gear::ecs::Query().all<Player, gear::Transform>(), chunkArray, chunkArraySize);

    for(auto c : chunks) {
        auto chunk = gear::ecs::ChunkView<gecs::EntityRef, Player, gear::Transform>(*c);

        for(auto [entity, player, transform] : chunk) {
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
                cmd.createEntity( gear::Sprite(player.bulletSprite),
                                  player.bulletShape,
                                  gear::Transform{transform.pos + glm::vec2(player.shootSideOffset, 24)},
                                  Bullet{{0, 2}}
                );
                player.shootSideOffset *= -1;

                player.shootTimer = 12;
            }
        }

    }

    chunks = ecs.queryChunks(gear::ecs::Query().all<gear::Transform, Bullet>(), chunkArray, chunkArraySize);
    for(auto c : chunks) {
        auto chunk = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::Transform, Bullet>(*c);
        for (auto[entity, transform, bullet] : chunk) {
            transform.pos += bullet.vel;
            if (transform.pos.y > 720) {
                cmd.destroyEntity(entity);
            }
        }
    }

    chunks = ecs.queryChunks(gear::ecs::Query().all<Velocity, gear::Transform>(), chunkArray, chunkArraySize);
    for(auto c: chunks) {
        auto chunk = gear::ecs::ChunkView<Velocity, gear::Transform>(*c);
        for (auto [velocity, transform] : chunk) {
            transform.pos += velocity.v;
        }
    }

    chunks = ecs.queryChunks(gear::ecs::Query().all<Enemy, gear::Transform>(), chunkArray, chunkArraySize);
    for(auto c: chunks) {
        auto chunk = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::Transform>(*c);
        for (auto [entity, transform] : chunk) {
            if (transform.pos.y < 32) {
                cmd.destroyEntity(entity);
            }
        }
    }
}

static void processCollisions(gear::ecs::Registry& ecs, gear::ecs::CommandEncoder& cmd, gear::AssetRegistry& assets) {
    const gear::ecs::Archetype enemyArch = gear::ecs::Archetype::create<Enemy>();
    const gear::ecs::Archetype bulletArch = gear::ecs::Archetype::create<Bullet>();

    gecs::Chunk* chunkArray[256];
    auto chunks = ecs.queryChunks(gecs::Query().all<CollisionPair>(), chunkArray, 256);
    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::EntityRef, CollisionPair>(*c);
        for (auto[collisionEntity, collision] : chunk) {
            if (auto entities = collision.get(enemyArch, bulletArch)) {
                auto[enemy, t] = entities->first.get<Enemy, gear::Transform>();

                if (--enemy.health <= 0) {
                    score += 100;
                    cmd.destroyEntity(entities->first);
                    cmd.createEntity(t, gear::ecs::CopyProvider{*assets.getSprite("explosion_0")}, DestroyOnAnimationEnd{});
                    cmd.createEntity(gear::Transform{t.pos + glm::vec2(-25, 25)},
                                     Text{"100", assets.getFont("default")},
                                     Lifetime{1});
                }
                cmd.destroyEntity(entities->second);

            }
            cmd.destroyEntity(collisionEntity);
        }
    };
}

static void processLifetime(gear::ecs::Registry& ecs, gear::ecs::CommandEncoder& cmd) {
    gecs::Chunk* chunkArray[256];
    auto chunks = ecs.queryChunks(gecs::Query().all<Lifetime>(), chunkArray, 256);
    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::EntityRef, Lifetime>(*c);
        for (auto[entity, lifetime] : chunk) {
            lifetime.time -= 1 / 60.f;
            if (lifetime.time <= 0) {
                cmd.destroyEntity(entity);
            }
        }
    };
}

static void processAnimation(gear::ecs::Registry& ecs, gear::ecs::CommandEncoder& cmd) {
    gecs::Chunk* chunkArray[1024];
    auto chunks = ecs.queryChunks(gecs::Query().all<gear::Sprite>(), chunkArray, 1024);

    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gear::Sprite>(*c);
        for (auto[sprite] : chunk) {
            sprite.imageIndex++;
            if (sprite.imageIndex >= sprite.texRegions.size()) {
                sprite.imageIndex = 0;
            }
        }
    }

    chunks = ecs.queryChunks(gecs::Query().all<gear::Sprite, DestroyOnAnimationEnd>(), chunkArray, 1024);
    for(auto c: chunks) {
        auto chunk = gecs::ChunkView<gecs::EntityRef, gear::Sprite>(*c);
        for (auto[entity, sprite] : chunk) {
            if (sprite.imageIndex == 0) {
                cmd.destroyEntity(entity);
            }
        }
    }
}

static void spawnEnemy(gecs::Prefab prefab, gear::AssetRegistry& assets, gear::ecs::CommandEncoder& cmd) {
    cmd.createEntity(prefab,
            gear::Transform{{480.0f * (rand()/(float)RAND_MAX), 720}},
            Velocity{{0, -1.5 - 0.5*(rand() / (float)RAND_MAX)}});
}

void render(gear::SpriteBatch& batch, gear::AssetRegistry& assets, gear::ecs::Registry& ecs) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    const size_t chunkArraySize = 1024;
    gecs::Chunk* chunkArray[chunkArraySize];

    gear::View view {{0,0}, {480, 720}};

    //tiles
    {
        auto shd = assets.getShader("textured");
        gear::tilemapSystemRender(ecs, shd.get());
    }

    {
        auto shd = assets.getShader("textured");
        gear::renderSprites(ecs, batch, shd.get());
    }

    {
        auto font = assets.getFont("default");
        auto shd = assets.getShader("font");
        shd->use();
        glUniform1i(shd->uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        auto chunks = ecs.queryChunks(gecs::Query().all<Text, gear::Transform>(), chunkArray, chunkArraySize);
        for(auto c : chunks){
            auto chunk = gecs::ChunkView<gear::Transform, Text>(*c);
            for (auto[transform, text] : chunk) {
                gear::renderText(text.text, *text.font, transform.pos, batch);
            }
        };

        gear::renderText("Score: " + std::to_string(score), *font, glm::vec2(20, 680), batch);

        batch.flush();
    }

}

void debugDraw(gear::PrimDraw& dd, gear::AssetRegistry& assets, gear::ecs::Registry& ecs) {
    gear::renderDebugShapes(ecs, dd, *assets.getShader("prim"));
}

void submitCommandBuffer(gecs::CommandBuffer& cmd, gecs::CommandEncoder& encoder, gecs::Registry& ecs) {
    gecs::executeCommandBuffer(cmd, ecs);
    gecs::resetCommandBuffer(cmd);
    encoder.reset();
}

gear::ecs::Prefab createEnemyPrefab(gear::ecs::Registry& reg, gear::AssetRegistry& assets, gear::ecs::CommandEncoder& encoder) {
    auto sprite = *assets.getSprite("ship1");
    gecs::EntityRef e = encoder.createEntity(
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

        assets->loadBundle("assets.bin");

        enemyPrefab = createEnemyPrefab(prefabs, *assets, cmdEncoder);
        submitCommandBuffer(cmd, cmdEncoder, prefabs);

        createStage(*assets, cmdEncoder);

        gear::ui::initialize(app->window);
    }

    void update() override {

        movePlayer(app, world, cmdEncoder);
        checkCollisions(world, cmdEncoder);
        submitCommandBuffer(cmd, cmdEncoder, world);
        processCollisions(world, cmdEncoder, *assets);
        render(*batch, *assets, world);
        debugDraw(*primDraw, *assets, world);
        processAnimation(world, cmdEncoder);
        processLifetime(world, cmdEncoder);
        submitCommandBuffer(cmd, cmdEncoder, world);

        if (--spawnTimer <= 0) {
            spawnEnemy(enemyPrefab, *assets, cmdEncoder);
            spawnTimer = 20;
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
    }

private:

    gear::ecs::Registry world;
    gear::ecs::Registry prefabs;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};
    gear::ecs::CommandEncoder cmdEncoder{cmd};

    gear::ecs::Prefab enemyPrefab;

    std::optional<gear::AssetRegistry> assets;
    std::optional<gear::SpriteBatch> batch;
    std::optional<gear::PrimDraw> primDraw;

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