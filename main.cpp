#include <iostream>

#include <gear/Application.h>
#include <glad/glad.h>
#include <gear/SpriteBatch.h>
#include <gear/TextureAtlas.h>
#include <memory>
#include <gear/Shader.h>
#include <gear/CollisionShape.h>
#include <gear/CollisionDetection.h>
#include <vector>
#include <algorithm>

std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texcoord;

out vec2 uv;
void main()
{
    gl_Position = vec4(Position, 0, 1);
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
    void init() override {
        batch = std::make_unique<gear::SpriteBatch>(100);
        tex = std::make_unique<gear::TextureAtlas>("out.json");
        shader = std::make_unique<gear::Shader>(vertexSource, fragmentSource);

        spr[0] = tex->getSprite("potato.png");
        spr[1] = tex->getSprite("potato2.png");
    }

    void update() override {

        dropTimer--;
        if (dropTimer <= 0) {
            dropTimer = 60;
            drops.push_back({{0, 1},
                             gear::Circle{{0,0}, 0.1},
                             spr[rand() % 2]});
        }

        for(auto& d : drops) {
            d.pos.y -= 0.01;
        }

        drops.erase(std::remove_if(drops.begin(), drops.end(),
                [](Drop& d){return d.pos.y < -1;}), drops.end());

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader->use();
        glUniform1i(shader->uniformLocation("tex"), 0);

        for(auto& d : drops) {
            batch->draw(d.spr, d.pos - glm::vec2{0.05, 0.05}, {0.1, 0.1});
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


    std::unique_ptr<gear::SpriteBatch> batch;
    std::unique_ptr<gear::Shader> shader;
    std::unique_ptr<gear::TextureAtlas> tex;
    gear::Sprite spr[2];
};



int main() {

    gear::AppConfig config {
        640, 480,"Game"
    };

    Game game;
    gear::run(config, game);

    return 0;
}