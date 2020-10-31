//
// Created by felix on 31/10/2020.
//

#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/g3d/g3d.h>

class Game : public gear::ApplicationAdapter {
public:

    void init(gear::Application *_app) override {
    }

    void update() override {
    }

    void end() override {
    }

private:

};

int main() {

    gear::AppConfig config {
            .width = 480,
            .height = 720,
            .title ="3d",
            .gapi = gear::g3dGetGapi()
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}