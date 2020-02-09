//
// Created by felix on 08/01/2020.
//

#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/DI.h>
#include <gear/TextureAtlas.h>
#include <gear/SpriteBatch.h>
#include <glm/vec2.hpp>
#include <gear/CollisionDetection.h>
#include <gear/CollisionShape.h>
#include <gear/Shader.h>
#include <gear/View.h>
#include <glm/gtc/type_ptr.hpp>
#include <gear/ECS/ECS.h>
#include <gear/CoreComponents.h>
#include <gear/ECS/System.h>

std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texcoord;

uniform mat4 view;

out vec2 uv;
void main()
{
    gl_Position = view * vec4(Position, 0, 1);
    uv = Texcoord;
}
)";

std::string fragmentSource = R"(
#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D tex;
void main(){
  color = texture(tex, uv);
}
)";

struct Player {
    float moveSpeed = 4;
    gear::Sprite bulletSprite;
    gear::CollisionShape  bulletShape;
    float shootTimer = 0;
};

struct Danger {
};

struct Bullet {
    glm::vec2 vel;
};

static void createStage(gear::TextureAtlas& atlas, gear::ecs::World& world) {

    gear::ecs::CommandBuffer cmd;

    {
        gear::Sprite spr = atlas.getSprite("ship1");
        for (int i = 0; i < 5; i++) {

            cmd.createEntity(spr,
                    gear::Transform{{480 * i / 5, 600}},
                    gear::CollisionShape{gear::Rectangle{{0,0}, {spr.size}}},
                    Danger{}
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

    world.executeCommandBuffer(cmd);
}

class PlayerMoveSystem : public gear::ecs::IteratingSystem<PlayerMoveSystem, Player, gear::Transform> {
public:

    union {
        std::tuple<gear::Application *> di{};
        struct {
            gear::Application* app;
        };
    };

    void execute(Player& player, gear::Transform& transform) {

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

            player.shootTimer = 0;

            /*
            auto e = world.create<gear::Sprite, gear::CollisionShape, gear::Transform, Bullet>();
            world.get<gear::Sprite>(e) = player.bulletSprite;
            world.get<gear::CollisionShape>(e) = player.bulletShape;
            world.get<gear::Transform>(e).pos = transform.pos + glm::vec2{0, 24};
            world.get<Bullet>(e).vel = {0, 10};

            player.shootTimer = 12;
             */
        }
    }

};
static void movePlayer(gear::Application* app, gear::ecs::World& world) {
    auto chunks = world.getChunks<Player, gear::Transform>();
    for(auto chunk : chunks) {
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

                auto e = world.create<gear::Sprite, gear::CollisionShape, gear::Transform, Bullet>();
                world.get<gear::Sprite>(e) = player.bulletSprite;
                world.get<gear::CollisionShape>(e) = player.bulletShape;
                world.get<gear::Transform>(e).pos = transform.pos + glm::vec2{0, 24};
                world.get<Bullet>(e).vel = {0, 10};

                player.shootTimer = 12;
            }

        }
    }


    for (auto chunk : world.getChunks<gear::Transform, Bullet>()) {
        for(auto [transform, bullet] : chunk) {
            transform.pos += bullet.vel;
        }
    }
}

static void checkCollisions(gear::ecs::World& world) {

    for(auto chunkA : world.getChunks<gear::Transform, gear::CollisionShape, Bullet>()) {
        for(auto [transformA, shapeA, bullet] : chunkA) {

            for(auto chunkB : world.getChunks<gear::Transform, gear::CollisionShape, Danger>()) {
                for(auto [transformB, shapeB, danger] : chunkB) {

                    if (gear::collide(shapeA, transformA.pos, shapeB, transformB.pos)) {

                    }

                }
            }
        }
    }



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

    for(auto chunk : ecsWorld.getChunks<gear::Sprite, gear::Transform>())
    {
        for(auto [sprite, transform] : chunk) {
            batch.draw(sprite, transform.pos);
        }
    }

    batch.flush();
}


class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *app) override {
        di.emplace<gear::ecs::World>();
        di.emplace<gear::TextureAtlas>("sprites.json");
        di.emplace<gear::SpriteBatch>(500);
        di.emplace<gear::Shader>(vertexSource, fragmentSource);
        di.emplace<gear::Application*>(app);

        di.invoke(createStage);
    }

    void update() override {
        PlayerMoveSystem p;
        di.inject(p);
        gear::ecs::execute(p, di.get<gear::ecs::World>());

        di.invoke(checkCollisions);
        di.invoke(render);
    }

    void end() override {
        di.reset();
    }

private:
    gear::DI di;
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