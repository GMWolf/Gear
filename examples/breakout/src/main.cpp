//
// Created by felix on 07/06/2020.
//
#include <gear/ApplicationAdapter.h>
#include <gear/Application.h>
#include <gear/ecs/ECS.h>
#include <gear/Assets.h>
#include <gear/g2d/RenderSystem.h>
#include <gear/Transform.h>
#include <gear/View.h>
#include <gear/CollisionDetection.h>
#include <gear/Input.h>
#include <gear/g2d/g2d.h>
#include <gear/g2d/Sprite.h>

struct Bat {float spd = 4;};
struct Ball {glm::vec2 v;};
struct Brick {};

void moveBat(gear::ecs::Registry& ecs, gear::Application* app) {
    for(auto c : ecs.query().all<Bat, gear::Transform, gear::CollisionShape>()) {
        auto chunk = gear::ecs::ChunkView<Bat, gear::Transform, gear::CollisionShape>(*c);
        for(auto [bat, transform, col] : chunk) {

            auto bbox = std::get<gear::Rectangle>(col);
            if (app->getInputState().keyDown(gear::KEYS::LEFT) && transform.pos.x + bbox.min.x > 0) {
                transform.pos.x -= bat.spd;
            }

            if (app->getInputState().keyDown(gear::KEYS::RIGHT) && transform.pos.x + bbox.max.x <= 720.0f) {
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
    const gear::assets::Sprite* spr = assets.getSprite("brick");
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 15; j++) {
            cmd.createEntity(
                    gear::Transform{{20 + j * 48,480 - (32 + i * 24)}},
                    gear::SpriteComponent{spr},
                    gear::getCollisionMask(spr),
                    Brick{});
        }
    }
}

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *_app) override {
        app = _app;
        assets.emplace();
        g2d = new gear::G2DInstance({
            .spriteBatchCreateInfo = {
                    .batchSize = 100
            }
        });
        assets->loadBundle("assets.bin");

        auto ballSpr = assets->getSprite("ball");
        cmd.createEntity(gear::Transform{{400, 80}},
                         gear::getCollisionMask(ballSpr),
                         gear::SpriteComponent{ballSpr},
                         Ball{{-1, -1}}
                         );

        auto batSpr = assets->getSprite("bat");
        cmd.createEntity(gear::Transform{{720 / 2, 30}},
                         gear::getCollisionMask(batSpr),
                         gear::SpriteComponent{batSpr},
                         Bat{});

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

        g2d->clearColor({0,0,0,1});
        g2d->setBlendMode();
        g2d->setCullFace(false);

        auto shd = assets->getShader("textured");
        gear::renderSprites(g2d, world, shd, cmd);
    }

    void end() override {
        delete g2d;
        assets.reset();
    }

private:
    gear::ecs::Registry world;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};
    std::optional<gear::AssetRegistry> assets;
    gear::G2DInstance* g2d;
    gear::Application* app {};
};

int main() {

    gear::AppConfig config = {
            720, 480,
            "shmup",
            gear::g2dGetGapi()
    };

    Game game;
    gear::Application app(config);
    app.run(game);
    return 0;
}