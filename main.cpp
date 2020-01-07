#include <iostream>

#include <gear/Application.h>
#include <glad/glad.h>
#include <gear/SpriteBatch.h>
#include <gear/TextureAtlas.h>
#include <memory>
#include <gear/Shader.h>


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

        a = tex->getSprite("potato.png");
        b = tex->getSprite("potato2.png");
    }

    void update() override {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glUniform1i(shader->uniformLocation("tex"), 0);

        batch->draw(a, {0, 0}, {0.5, 0.5});
        batch->draw(b, {-0.4, 0.2}, {0.1, 0.1});
        batch->flush();

    }

    void end() override {
        batch.reset();
        tex.reset();
        shader.reset();
    }

    std::unique_ptr<gear::SpriteBatch> batch;
    std::unique_ptr<gear::Shader> shader;
    std::unique_ptr<gear::TextureAtlas> tex;
    gear::Sprite a;
    gear::Sprite b;
};



int main() {

    gear::AppConfig config {
        640, 480,"Game"
    };

    Game game;
    gear::run(config, game);

    return 0;
}