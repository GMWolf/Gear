//
// Created by felix on 31/10/2020.
//

#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/g3d/g3d.h>
#include <gear/Assets.h>
#include <optional>

class Game : public gear::ApplicationAdapter {
public:

    void init(gear::Application *_app) override {
        assets.emplace();
        assets->loadBundle("assets.bin");
        g3d = new gear::G3DInstance();
    }

    void update() override {
        //g3d->debugTexture(assets->getTexture("Fabric_Boucle"), assets->getShader("testshd"));
        g3d->debugMesh(assets->getMesh("SciFiHelmet"), assets->getTexture("Fabric_Boucle"), assets->getShader("defaultShd"));
    }

    void end() override {
        assets.reset();
        delete g3d;
    }

private:

    gear::G3DInstance* g3d;
    std::optional<gear::AssetRegistry> assets;

};

int main() {

    gear::AppConfig config {
            .width = 720,
            .height = 720,
            .title ="3d",
            .gapi = gear::g3dGetGapi()
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}