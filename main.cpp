#include <iostream>

#include <gear/Application.h>
#include <glad/glad.h>

class Game : public gear::ApplicationAdapter {
public:
    void init() override {

    }

    void update() override {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


    }

};



int main() {

    gear::AppConfig config {
        640, 480,"Game"
    };

    Game game;
    gear::run(config, game);

    return 0;
}