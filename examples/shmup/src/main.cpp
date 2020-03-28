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
    std::shared_ptr<const gear::BitmapFont> font;
};

struct Lifetime {
    float time;
};

static int score = 0;

static void createStage(gear::AssetManager& assets, gear::ecs::CommandEncoder& cmd) {

    auto atlas = assets.get_as<gear::TextureAtlas>("shmup_textures.json");
    {
        gear::Sprite spr = atlas->getSprite("ship1");
        for (int i = 0; i < 5; i++) {

            cmd.createEntity(gear::Sprite(spr),
                             gear::Transform{{480 * i / 5, 600}},
                             gear::CollisionShape{gear::Rectangle{
                                 {spr.bbox.left,spr.bbox.bottom},
                                 {spr.bbox.right, spr.bbox.top}}},
                             Enemy{}
                    );
        }
    }

    {
        gear::Sprite spr = atlas->getSprite("ship2");

        Player player;
        player.bulletSprite = atlas->getSprite("bullet_blue");
        player.bulletShape = gear::Rectangle{{player.bulletSprite.bbox.left,player.bulletSprite.bbox.bottom},
                                             {player.bulletSprite.bbox.right, player.bulletSprite.bbox.top}};

        cmd.createEntity( spr,
                gear::Transform{{480 / 2, 32}},
                gear::CollisionShape{gear::Rectangle{{spr.bbox.left,spr.bbox.bottom}, {spr.bbox.right, spr.bbox.top}}},
                player);

    }

    //collision filters
    {
        cmd.createEntity(CollisionFilter{
            gear::ecs::Query().all<Enemy>(),
            gear::ecs::Query().all<Bullet>()
        });
    }
}

static void movePlayer(gear::Application* app, gear::ecs::World& world, gear::ecs::CommandEncoder& cmd) {

    const size_t chunkArraySize = 256;
    gear::ecs::Chunk* chunkArray[chunkArraySize];
    auto chunks = world.queryChunks(gear::ecs::Query().all<Player, gear::Transform>(), chunkArray, chunkArraySize);

    for(auto c : chunks) {
        auto chunk = gear::ecs::ChunkView<Player, gear::Transform>(*c);

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

                cmd.createEntity( gear::Sprite(player.bulletSprite),
                                  player.bulletShape,
                                  gear::Transform{transform.pos + glm::vec2(0, 24)},
                                  Bullet{{0, 10}}
                );

                player.shootTimer = 1;
            }
        }

    }

    chunks = world.queryChunks(gear::ecs::Query().all<gear::Transform, Bullet>(), chunkArray, chunkArraySize);
    for(auto c : chunks) {
        auto chunk = gear::ecs::ChunkView<gear::ecs::Entity, gear::Transform, Bullet>(*c);
        for (auto[entity, transform, bullet] : chunk) {
            transform.pos += bullet.vel;
            if (transform.pos.y > 720) {
                cmd.destroyEntity(entity);
            }
        }
    }

    chunks = world.queryChunks(gear::ecs::Query().all<Enemy, gear::Transform>(), chunkArray, chunkArraySize);
    for(auto c: chunks) {
        auto chunk = gear::ecs::ChunkView<gear::ecs::Entity, Enemy, gear::Transform>(*c);
        for (auto [entity, enemy, transform] : chunk) {
            if (transform.pos.y > 32) {
                transform.pos.y -= 2;
            } else {
                cmd.destroyEntity(entity);
            }
        }
    }
}

static void processCollisions(gear::ecs::World& world, gear::ecs::CommandEncoder& cmd, const gear::AssetManager& assetManager) {
    const gear::ecs::Archetype enemyArch = gear::ecs::Archetype::create<Enemy>();
    const gear::ecs::Archetype bulletArch = gear::ecs::Archetype::create<Bullet>();
    auto atlas = assetManager.get_as<gear::TextureAtlas>("shmup_textures.json");

    gecs::Chunk* chunkArray[256];
    auto chunks = world.queryChunks(gecs::Query().all<CollisionPair>(), chunkArray, 256);
    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::Entity, CollisionPair>(*c);
        for (auto[collisionEntity, collision] : chunk) {
            if (auto entities = collision.get(enemyArch, bulletArch)) {
                auto[enemy, t] = world.get<Enemy, gear::Transform>(entities->first);

                if (--enemy.health <= 0) {
                    score += 100;
                    cmd.destroyEntity(entities->first);
                    cmd.createEntity(t, atlas->getSprite("explosion"), DestroyOnAnimationEnd{});
                    cmd.createEntity(gear::Transform{t.pos - glm::vec2(25, 0)},
                                     Text{"100", assetManager.get_as<gear::BitmapFont>("shmup_default_font.json")},
                                     Lifetime{1});
                }
                cmd.destroyEntity(entities->second);
            }
            cmd.destroyEntity(collisionEntity);
        }
    };
}

static void processLifetime(gear::ecs::World& world, gear::ecs::CommandEncoder& cmd) {
    gecs::Chunk* chunkArray[256];
    auto chunks = world.queryChunks(gecs::Query().all<Lifetime>(), chunkArray, 256);
    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::Entity, Lifetime>(*c);
        for (auto[entity, lifetime] : chunk) {
            lifetime.time -= 1 / 60.f;
            if (lifetime.time <= 0) {
                cmd.destroyEntity(entity);
            }
        }
    };
}


static void processAnimation(gear::ecs::World& world, gear::ecs::CommandEncoder& cmd) {
    gecs::Chunk* chunkArray[1024];
    auto chunks = world.queryChunks(gecs::Query().all<gear::Sprite>(), chunkArray, 1024);

    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gear::Sprite>(*c);
        for (auto[sprite] : chunk) {
            sprite.imageIndex++;
            if (sprite.imageIndex >= sprite.texRegions.size()) {
                sprite.imageIndex = 0;
            }
        }
    }

    chunks = world.queryChunks(gecs::Query().all<gear::Sprite, DestroyOnAnimationEnd>(), chunkArray, 1024);
    for(auto c: chunks) {
        auto chunk = gecs::ChunkView<gecs::Entity, gear::Sprite>(*c);
        for (auto[entity, sprite] : chunk) {
            if (sprite.imageIndex == 0) {
                cmd.destroyEntity(entity);
            }
        }
    }
}

static void spawnEnemy(gear::AssetManager& assetManager, gear::ecs::CommandEncoder& cmd) {
    auto atlas = assetManager.get_as<gear::TextureAtlas>("shmup_textures.json");
    gear::Sprite spr = atlas->getSprite("ship1");
    cmd.createEntity(spr,
                     gear::Transform{{480.0f * (rand()/(float)RAND_MAX), 720+spr.size.y}},
                     gear::CollisionShape{gear::Rectangle{{spr.bbox.left,spr.bbox.bottom}, {spr.bbox.right, spr.bbox.top}}},
                     Enemy{}
    );

}

void render(gear::SpriteBatch& batch, gear::AssetManager& assets, gear::ecs::World& ecsWorld) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    const size_t chunkArraySize = 1024;
    gecs::Chunk* chunkArray[chunkArraySize];

    gear::View view {{0,0}, {480, 720}};

    {
        auto shd = assets.get_as<gear::Shader>("simple_textured");

        shd->use();
        glUniform1i(shd->uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        auto chunks = ecsWorld.queryChunks(gecs::Query().all<gear::Sprite, gear::Transform>(), chunkArray, chunkArraySize);

        for(auto c : chunks) {
            auto chunk = gecs::ChunkView<gear::Sprite, gear::Transform>(*c);
            for (auto[sprite, transform] : chunk) {
                batch.draw(sprite, transform.pos);
            }
        };

        batch.flush();

    }


    {
        auto& font = *assets.get_as<gear::BitmapFont>("shmup_default_font.json");
        auto shd = assets.get_as<gear::Shader>("shd_font");
        shd->use();
        glUniform1i(shd->uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        auto chunks = ecsWorld.queryChunks(gecs::Query().all<Text, gear::Transform>(), chunkArray, chunkArraySize);
        for(auto c : chunks){
            auto chunk = gecs::ChunkView<gear::Transform, Text>(*c);
            for (auto[transform, text] : chunk) {
                gear::renderText(text.text, *text.font, transform.pos, batch);
            }
        };


        gear::renderText("Score: " + std::to_string(score), font, glm::vec2(20, 680), batch);

        batch.flush();
    }
}

void submitCommandBuffer(gecs::CommandBuffer& cmd, gecs::CommandEncoder& encoder, gecs::World& world) {
    gecs::executeCommandBuffer(cmd, world);
    gecs::resetCommandBuffer(cmd);
    encoder.reset();
}

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *_app) override {
        app = _app;
        di.emplace<gear::ecs::World>();
        di.emplace<gear::AssetManager>();
        di.emplace<gear::SpriteBatch>(500);
        di.emplace<gear::Application*>(app);
        auto& cmdbuff = di.emplace<gear::ecs::CommandBuffer>(256'000'000);
        di.emplace<gear::ecs::CommandEncoder>(cmdbuff);
        auto& assetManager = di.emplace<gear::AssetManager>();

        assetManager.setLoader<gear::Shader, gear::ShaderLoader>();
        assetManager.setLoader<gear::BitmapFont, gear::BitmapFontLoader>();
        assetManager.setLoader<gear::TextureAtlas, gear::TextureAtlasLoader>();

        assetManager.load<gear::Shader>("simple_textured");
        assetManager.load<gear::Shader>("shd_font");
        assetManager.load<gear::TextureAtlas>("shmup_textures.json");
        assetManager.load<gear::BitmapFont>("shmup_default_font.json");

        di.invoke(createStage);

        gear::ui::initialize(app->window);
    }

    void update() override {

        di.invoke(movePlayer);
        di.invoke(checkCollisions);
        di.invoke(submitCommandBuffer);
        di.invoke(processCollisions);
        di.invoke(render);
        di.invoke(processAnimation);
        di.invoke(processLifetime);
        di.invoke(submitCommandBuffer);
        if (--spawnTimer <= 0) {
            di.invoke(spawnEnemy);
            spawnTimer = 1;
        }


        //gear::ui::begin();
        //perf.frameTime = app->frameTime;
        //gear::ui::perfWindow(perf);
        //gear::ui::end();
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

    gear::AppConfig config {
        480, 720,
        "shmup",
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}