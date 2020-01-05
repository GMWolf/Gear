#include <iostream>

#include <gear/Application.h>
#include <glad/glad.h>
#include <gear/SpriteBatch.h>
#include <gear/Texture.h>
#include <memory>


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

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        const char* vss = vertexSource.c_str();
        glShaderSource(vs, 1, &vss, nullptr);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fss = fragmentSource.c_str();
        glShaderSource(fs, 1, &fss, nullptr);
        glCompileShader(fs);

        program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        glDetachShader(program, vs);
        glDetachShader(program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        tex = std::make_unique<gear::Texture>("potato.jpg");
    }

    void update() override {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);

        batch->draw(*tex, {0, 0}, {0.5, 0.5});
        batch->draw(*tex, {-0.4, 0.2}, {0.1, 0.1});
        batch->flush();

    }

    void end() override {
        batch.reset();
    }

    std::unique_ptr<gear::SpriteBatch> batch;
    GLuint program = 0;
    std::unique_ptr<gear::Texture> tex;
};



int main() {

    gear::AppConfig config {
        640, 480,"Game"
    };

    Game game;
    gear::run(config, game);

    return 0;
}