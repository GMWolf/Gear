//
// Created by felix on 08/01/2020.
//

#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/World.h>
#include <gear/TextureAtlas.h>
#include <gear/SpriteBatch.h>
#include <glm/vec2.hpp>
#include <gear/CollisionDetection.h>
#include <gear/CollisionShape.h>

struct Gear {
    glm::vec2 pos;
    gear::Sprite sprite;
    gear::CollisionShape shape;
};


class Game : gear::ApplicationAdapter {
public:
    void init(gear::Application *app) override {
        world.emplace<gear::TextureAtlas>("sprites.json");
        world.emplace<gear::SpriteBatch>(500);
        world.emplace<std::vector<Gear>>();
    }

    static void render(gear::SpriteBatch& batch, std::vector<Gear>& gear) {

    }

    void update() override {

    }

    void end() override {

    }

private:

    gear::World world;

};

int main() {

    gear::AppConfig config {
        1280, 720,
        "Gear hell",
    };


    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}