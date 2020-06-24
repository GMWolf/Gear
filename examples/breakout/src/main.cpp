//
// Created by felix on 07/06/2020.
//
#include <gear/ApplicationAdapter.h>
#include <gear/Application.h>
#include <gear/ECS/ECS.h>
#include <gear/AssetManager.h>
#include <gear/SpriteBatch.h>
#include <gear/RenderSystem.h>
#include <gear/Transform.h>
#include <gear/Texture.h>
#include <gear/View.h>
#include <gear/CollisionDetection.h>

struct Bat {float spd = 4;};
struct Ball {glm::vec2 v;};
struct Brick {};

void moveBat(gear::ecs::Registry& ecs, gear::Application* app) {
    for(auto c : ecs.query().all<Bat, gear::Transform, gear::CollisionShape>()) {
        auto chunk = gear::ecs::ChunkView<Bat, gear::Transform, gear::CollisionShape>(*c);
        for(auto [bat, transform, col] : chunk) {

            auto bbox = std::get<gear::Rectangle>(col);
            if (app->keyPressed(gear::KEYS::LEFT) && transform.pos.x + bbox.min.x > 0) {
                transform.pos.x -= bat.spd;
            }

            if (app->keyPressed(gear::KEYS::RIGHT) && transform.pos.x + bbox.max.x <= 720.0f) {
                transform.pos.x += bat.spd;
            }
        }
    }
}

bool ballCollide(gear::Transform& ballTransform, Ball& ball, gear::CollisionShape& ballShape, gear::ecs::Registry& ecs, gear::ecs::CommandBuffer& cmd, int axis)
{
    auto ballCircle = std::get<gear::Circle>(ballShape);
    for(auto c : ecs.query().all<gear::Transform, gear::CollisionShape>().none<Ball>()) {
        auto chunk = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::Transform, gear::CollisionShape>(*c);
        for(auto [e, t, col] : chunk) {
            if (gear::collide(ballShape, ballTransform.pos, col, t.pos)) {
                if (c->archetype[gear::ecs::Component<Brick>::ID()]) {
                    cmd.destroyEntity(e);
                }
                if (auto rect = std::get_if<gear::Rectangle>(&col)) {
                    if (ball.v[axis] > 0) {
                        ballTransform.pos[axis] = t.pos[axis] + rect->min[axis] - ballCircle.radius;
                    } else {
                        ballTransform.pos[axis] = t.pos[axis] + rect->max[axis] + ballCircle.radius;
                    }
                }
                ball.v[axis] *= -1;
            }
        }
    }
    return false;
}

void moveBall(gear::ecs::Registry& ecs, gear::ecs::CommandBuffer& cmd) {
    for(auto c : ecs.query().all<Ball, gear::Transform, gear::CollisionShape>()) {
        auto chunk = gear::ecs::ChunkView<Ball, gear::Transform, gear::CollisionShape>(*c);
        for(auto [ball, transform, col] : chunk) {
            transform.pos.y += ball.v.y;
            ballCollide(transform, ball, col, ecs, cmd, 1);
            transform.pos.x += ball.v.x;
            ballCollide(transform, ball, col, ecs, cmd, 0);
        }
    }
}

void createBricks(gear::ecs::CommandBuffer& cmd, gear::AssetRegistry& assets) {
    gear::Sprite spr = *assets.getSprite("brick");
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 15; j++) {
            cmd.createEntity(gear::Transform{{20 + j * 48,480 - (32 + i * 24)}},gear::ecs::CopyProvider{spr},gear::ecs::CopyProvider{*spr.mask},Brick{});
        }
    }
}

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *_app) override {
        app = _app;
        batch.emplace(100);
        assets.emplace();

        assets->loadBundle("assets.bin");

        auto batSpr = *assets->getSprite("bat");
        cmd.createEntity(gear::Transform{{720 / 2, 30}},*batSpr.mask,batSpr,Bat{});

        auto ballSpr = *assets->getSprite("ball");
        cmd.createEntity(gear::Transform{{400, 80}},*ballSpr.mask,ballSpr,Ball{{-1, -1}});

        cmd.createEntity(gear::View{{0, 0}, {720, 480}});
        cmd.createEntity(gear::Transform{{0,0}}, gear::CollisionShape{gear::Rectangle{glm::vec2{-10,0},glm::vec2{0, 480}}});
        cmd.createEntity(gear::Transform{{0,0}}, gear::CollisionShape{gear::Rectangle{glm::vec2{720,0},glm::vec2{730, 480}}});
        cmd.createEntity(gear::Transform{{0,0}}, gear::CollisionShape{gear::Rectangle{glm::vec2{0,480},glm::vec2{720, 490}}});
        createBricks(cmd, *assets);
    }

    void update() override {
        moveBat(world, app);
        moveBall(world, cmd);
        gear::ecs::executeCommandBuffer(cmd, world);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        gear::renderSprites(world, *batch, *assets->getShader("textured"));
    }

    void end() override {
        batch.reset();
        assets.reset();
    }

private:
    gear::ecs::Registry world;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};
    std::optional<gear::AssetRegistry> assets;
    std::optional<gear::SpriteBatch> batch;
    gear::Application* app {};
};

int main() {
    gear::AppConfig config {
            720, 480,
            "shmup",
    };
    Game game;
    gear::Application app(config);
    app.run(game);
    return 0;
}