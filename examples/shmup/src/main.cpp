//
// Created by felix on 08/01/2020.
//
#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/DI.h>
#include <gear/TextureAtlas.h>
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
#include <iostream>

#include "Collisions.h"

namespace gecs = gear::ecs;

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

struct Text {
    std::string text;
    gear::AssetReference<gear::BitmapFont> font;
};

struct Lifetime {
    float time;
};

struct Invisible {
    float time = 4;
};

struct Velocity {
    glm::vec2 v;
};

static int score = 0;

static void createStage(gear::AssetRegistry& assets, gear::ecs::CommandEncoder& cmd) {

    auto atlas = assets.get<gear::TextureAtlas>("shmup_textures.bin");
    {
        gear::Sprite spr = atlas->getSprite("ship1");
        for (int i = 0; i < 5; i++) {

            cmd.createEntity(gear::Sprite(spr),
                             gear::Transform{{480 * i / 5, 600}},
                             *spr.mask,
                             Enemy{},
                             Velocity{{0, -1.5}}
                    );
        }
    }

    {
        gear::Sprite spr = atlas->getSprite("ship2");

        Player player;
        player.bulletSprite = atlas->getSprite("bullet_blue1");
        player.bulletShape = *spr.mask;

        cmd.createEntity( spr,
                gear::Transform{{480 / 2, 32}},
                *spr.mask,
                player);

    }

    //tilemap
    if (false){
        auto map = assets.get<gear::TileMap>("../../../../examples/shmup/assets/maps/map1.tmx");
        cmd.createEntity(
                gear::TilemapComponent{map},
                gear::Transform{{0,0}},
                Velocity{{0, -1}}
                );
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

            if (app->keyPressed(gear::KEYS::Q)) {
                cmd.createComponent(entity, Invisible{40});
            }

            if (player.shootTimer > 0) player.shootTimer--;

            if (app->keyPressed(gear::KEYS::SPACE) && player.shootTimer <= 0) {

                cmd.createEntity( gear::Sprite(player.bulletSprite),
                                  player.bulletShape,
                                  gear::Transform{transform.pos + glm::vec2(0, 24)},
                                  Bullet{{0, 1}}
                );

                player.shootTimer = 6;
            }
        }

    }

    chunks = ecs.queryChunks(gear::ecs::Query().all<Invisible>(), chunkArray, chunkArraySize);
    for(auto c : chunks) {
        auto chunk = gear::ecs::ChunkView<gecs::EntityRef, Invisible>(*c);
        for(auto [entity, invisible] : chunk) {
            invisible.time -= 0.16;
            if (invisible.time <= 0) {
                cmd.destroyComponent<Invisible>(entity);
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
    auto atlas = assets.get<gear::TextureAtlas>("shmup_textures.bin");

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
                    cmd.createEntity(t, atlas->getSprite("explosion_0"), DestroyOnAnimationEnd{});
                    cmd.createEntity(gear::Transform{t.pos + glm::vec2(-25, 25)},
                                     Text{"100", assets.get<gear::BitmapFont>("shmup_default_font.bin")},
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

static void spawnEnemy(gear::AssetRegistry& assets, gear::ecs::CommandEncoder& cmd) {
    auto atlas = assets.get<gear::TextureAtlas>("shmup_textures.bin");
    gear::Sprite spr = atlas->getSprite("ship1");
    cmd.createEntity(spr,
                     gear::Transform{{480.0f * (rand()/(float)RAND_MAX), 720+spr.size.y}},
                     *spr.mask,
                     Enemy{},
                 Velocity{{0, -1.5 - 0.5*(rand() / (float)RAND_MAX)}}
    );

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
        auto shd = assets.get<gear::Shader>("simple_textured");
        gear::tilemapSystemRender(ecs, shd.get());
    }

    {
        auto shd = assets.get<gear::Shader>("simple_textured");
        gear::renderSprites(ecs, batch, shd.get());
    }

    {
        auto font = assets.get<gear::BitmapFont>("shmup_default_font.bin");
        auto shd = assets.get<gear::Shader>("shd_font");
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

void submitCommandBuffer(gecs::CommandBuffer& cmd, gecs::CommandEncoder& encoder, gecs::Registry& ecs) {
    gecs::executeCommandBuffer(cmd, ecs);
    gecs::resetCommandBuffer(cmd);
    encoder.reset();
}

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *_app) override {
        app = _app;
        di.emplace<gear::ecs::Registry>();
        di.emplace<gear::AssetRegistry>();
        di.emplace<gear::SpriteBatch>(500);
        di.emplace<gear::Application*>(app);
        auto& cmdbuff = di.emplace<gear::ecs::CommandBuffer>(256'000'000);
        di.emplace<gear::ecs::CommandEncoder>(cmdbuff);
        auto& assetManager = di.emplace<gear::AssetRegistry>();

        assetManager.setLoader<gear::Shader>(gear::ShaderLoader());
        assetManager.setLoader<gear::BitmapFont>(gear::BitmapFontLoader());
        assetManager.setLoader<gear::TextureAtlas>(gear::TextureAtlasLoader());
        assetManager.setLoader<gear::TileSet>(gear::TileSetLoader());
        assetManager.setLoader<gear::TileMap>(gear::TileMapLoader());
        assetManager.setLoader<gear::Texture>(gear::TextureLoader());

        assetManager.load<gear::Shader>("simple_textured");
        assetManager.load<gear::Shader>("shd_font");
        assetManager.load<gear::TextureAtlas>("shmup_textures.bin");
        assetManager.load<gear::BitmapFont>("shmup_default_font.bin");
        //assetManager.load<gear::TileMap>("../../../../examples/shmup/assets/maps/map1.tmx");

        di.invoke(createStage);

        gear::ui::initialize(app->window);
    }

    void update() override {

        di.invoke(movePlayer);
        di.invoke(checkCollisions);
        di.invoke(submitCommandBuffer);
        di.invoke(processCollisions);
        di.invoke(gear::tilemapSystemCreateSystemComponent);
        di.invoke(render);
        di.invoke(processAnimation);
        di.invoke(processLifetime);
        di.invoke(submitCommandBuffer);
        if (--spawnTimer <= 0) {
            di.invoke(spawnEnemy);
            spawnTimer = 20;
        }

        gear::ui::begin();
        perf.frameTime = app->frameTime;
        gear::ui::perfWindow(perf);
        gear::ui::end();
    }

    void end() override {
        di.reset();
        gear::ui::cleanup();
    }

private:
    gear::DI di;
    int spawnTimer = 10;
    gear::ui::PerfData perf {};
    gear::Application* app {};
};

int main() {

    std::cout << " yo! " << std::endl;

    gear::AppConfig config {
        480, 720,
        "shmup",
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}