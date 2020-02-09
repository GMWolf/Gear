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
#include <gear/DI.h>
#include <gear/ECS/ECS.h>

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

struct Drop {
    glm::vec2 pos;
    gear::CollisionShape shape;
    gear::Sprite spr;
};

struct Bat {
    glm::vec2 pos;
    gear::CollisionShape shape;
    gear::Sprite spr;
};

class Game : public gear::ApplicationAdapter {
public:
    void init(gear::Application* app) override {

        di.emplace<gear::SpriteBatch>(100);
        di.emplace<std::vector<Drop>>();
        di.emplace<gear::TextureAtlas>("out.json");

        auto& batch = di.get<gear::SpriteBatch>();

        this->app = app;
        di.emplace<gear::Shader>(vertexSource, fragmentSource);

        di.emplace<gear::View>(gear::View{{0, 0}, {640, 480}});

        auto tex = di.get<gear::TextureAtlas>();
        spr[0] = tex.getSprite("potato");
        spr[1] = tex.getSprite("potato2");

        gear::Sprite sprBat = tex.getSprite("bat");
        di.emplace<Bat>(Bat{
            {256, 32},
            gear::Rectangle{{0,0}, sprBat.size},
            sprBat
        });
    }

    void update() override {

        auto& drops = di.get<std::vector<Drop>>();
        auto& bat = di.get<Bat>();

        dropTimer--;
        if (dropTimer <= 0) {
            dropTimer = 30;
            di.invoke(spawnDrop);
        }

        di.invoke(updateDrops);

        if (app->keyPressed(gear::KEYS::LEFT)) {
            bat.pos.x -= 4;
        }
        if (app->keyPressed(gear::KEYS::RIGHT)) {
            bat.pos.x += 4;
        }

        di.invoke(render);
    }

    static void render(gear::SpriteBatch& batch, gear::Shader& shader, gear::View& view, std::vector<Drop>& drops, Bat& bat) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader.use();
        glUniform1i(shader.uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shader.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        for(auto& d : drops) {
            batch.draw(d.spr, d.pos);
        }
        batch.draw(bat.spr, bat.pos);
        batch.flush();
    }


    static void updateDrops(std::vector<Drop>& drops, Bat& bat) {
        for(auto& d : drops) {
            d.pos.y -= 4;
        }

        gear::Rectangle floor {{0,-1}, {640, 0}};

        drops.erase(std::remove_if(drops.begin(), drops.end(),
                                   [&](Drop& d){
                                       return gear::collide(d.shape, d.pos, floor, {0,-150}) |
                                              gear::collide(d.shape, d.pos, bat.shape, bat.pos);
                                   }), drops.end());
    }

    static void spawnDrop(std::vector<Drop>& drops, gear::TextureAtlas& atlas) {
        auto s = rand()%2 == 0? atlas.getSprite("potato") : atlas.getSprite("potato2");
        s.size *= 0.5;
        drops.push_back({{rand() % 640, 480},
                         gear::Rectangle{{0,0}, s.size},
                         s});
    }

    void end() override {
        di.reset();
    }

    gear::DI di;
    int dropTimer = 60;

    gear::Sprite spr[2];

    gear::Application* app = nullptr;
};



int main_() {

    gear::AppConfig config {
        640, 480,"Game"
    };


    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}


int main() {

    struct A {
        int a = 4, b;
    };

    struct B {
        int c = 42;
        float d;
    };

    struct C {
        int a;
    };

    gear::ecs::World world;

    auto e = world.create<A, B>();
    std::cout << e << std::endl;
    world.get<A>(e).a = 900;

    e = world.create<A, B>();
    std::cout << e << std::endl;
    world.get<B>(e).c = 33;


    for(auto chunk : world.getChunks<A, B>()) {
        for(auto [a, b] : chunk) {
            std::cout << a.a << " " << b.c << std::endl;
        }
    }

    return 0;

}