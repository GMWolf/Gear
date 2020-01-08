#include <iostream>

#include <gear/Application.h>
#include <glad/glad.h>
#include <gear/SpriteBatch.h>
#include <gear/TextureAtlas.h>
#include <memory>
#include <gear/Shader.h>
#include <gear/CollisionShape.h>
#include <gear/CollisionDetection.h>
#include <gear/View.h>
#include <vector>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

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


class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application* app) override {
        this->app = app;
        batch = std::make_unique<gear::SpriteBatch>(100);
        tex = std::make_unique<gear::TextureAtlas>("out.json");
        shader = std::make_unique<gear::Shader>(vertexSource, fragmentSource);

        spr[0] = tex->getSprite("potato.png");
        spr[1] = tex->getSprite("potato2.png");

        view.pos = {0,0};
        view.size = {640, 480};
    }

    void update() override {
        dropTimer--;
        if (dropTimer <= 0) {
            dropTimer = 60;
            auto s = spr[rand() % 2];
            s.size *= 0.5;
            drops.push_back({{rand() % 640, 480},
                             gear::Rectangle{{0,0}, s.size},
                             s});
        }

        for(auto& d : drops) {
            d.pos.y -= 4;
        }

        gear::Rectangle floor {{0,-1}, {640, 0}};

        drops.erase(std::remove_if(drops.begin(), drops.end(),
                [floor](Drop& d){return gear::collide(d.shape, d.pos, floor, {0,0});}), drops.end());

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader->use();
        glUniform1i(shader->uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        for(auto& d : drops) {
            batch->draw(d.spr, d.pos);
        }
        batch->flush();
    }

    void end() override {
        batch.reset();
        tex.reset();
        shader.reset();
    }


    struct Drop {
        glm::vec2 pos;
        gear::CollisionShape shape;
        gear::Sprite spr;
    };

    std::vector<Drop> drops;
    int dropTimer = 60;

    gear::View view{};

    std::unique_ptr<gear::SpriteBatch> batch;
    std::unique_ptr<gear::Shader> shader;
    std::unique_ptr<gear::TextureAtlas> tex;
    gear::Sprite spr[2];

    gear::Application* app = nullptr;
};



int main() {

    gear::AppConfig config {
        640, 480,"Game"
    };


    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}