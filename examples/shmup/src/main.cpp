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
#include <gear/Shader.h>
#include <gear/View.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <optional>

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


template<class T> using Store = std::vector<std::optional<T>>;


using Position = glm::vec2;

static void createStage(gear::TextureAtlas& atlas, Store<gear::Sprite>& sprites, Store<Position>& pos) {
    {
        gear::Sprite spr = atlas.getSprite("ship1");
        for (int i = 0; i < 5; i++) {
           int n = glm::max(sprites.size(), pos.size());
           if (sprites.size() <= n) sprites.resize(n+1);
           if (pos.size() <= n) pos.resize(n+1);

            sprites[n] = spr;
            pos[n] = glm::vec2{480 * i / 5, 600};
        }
    }

    {
        int n = glm::max(sprites.size(), pos.size());
        if (sprites.size() <= n) sprites.resize(n+1);
        if (pos.size() <= n) pos.resize(n+1);

        sprites[n] = atlas.getSprite("player");
        pos[n] = {480 / 2, 32};
    }
}

void render(gear::SpriteBatch& batch, gear::Shader& shd, Store<gear::Sprite>& sprites, Store<Position>& pos) {
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

    int n = glm::min(sprites.size(), pos.size());

    for(int i = 0; i < n; i++) {
        if (sprites[i] && pos[i]) {
            batch.draw(*sprites[i], *pos[i]);
        }
    }

    batch.flush();
}
/*
void updatePlayer(gear::Application* app, Player& player) {
    glm::vec2 d{0,0};
    if (app->keyPressed(gear::KEYS::LEFT)) {
        d.x -= 1;
    }
    if (app->keyPressed(gear::KEYS::RIGHT)) {
        d.x += 1;
    }
    if (app->keyPressed(gear::KEYS::UP)) {
        d.y += 1;
    }
    if (app->keyPressed(gear::KEYS::DOWN)) {
        d.y -= 1;
    }
    if (d != glm::vec2{0,0}) {
        player.pos += glm::normalize(d) * 3.f;
    }
}
*/
class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application *app) override {
        world.emplace<gear::TextureAtlas>("sprites.json");
        world.emplace<gear::SpriteBatch>(500);
        world.emplace<Store<gear::Sprite>>();
        world.emplace<Store<Position>>();
        world.emplace<gear::Shader>(vertexSource, fragmentSource);
        world.emplace<gear::Application*>(app);


        world.invoke(createStage);
    }


    void update() override {
        world.invoke(render);
//        world.invoke(updatePlayer);
    }

    void end() override {
        world.reset();
    }

private:
    gear::World world;
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