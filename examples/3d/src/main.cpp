//
// Created by felix on 31/10/2020.
//

#include <gear/Application.h>
#include <gear/ApplicationAdapter.h>
#include <gear/g3d/g3d.h>
#include <gear/Assets.h>
#include <optional>
#include <gear/ecs/ECS.h>
#include <gear/Transform.h>
#include <gear/View.h>
#include <gear/Input.h>
#include <gear/mesh_generated.h>

class Game : public gear::ApplicationAdapter {
public:

    void init(gear::Application *app) override {
        application = app;
        assets.emplace();
        assets->loadBundle("assets.bin");
        assets->loadBundle("assets/models/SciFiHelmet/SciFiHelmet.bundle");
        assets->loadBundle("assets/models/Corset/Corset.bundle");

        g3d = new gear::G3DInstance();
        {
            gear::Camera camera{};
            camera.near = 0.1;
            camera.far = 1000;
            camera.fov = glm::radians(45.0f);
            camera.viewPort.pos = {0, 0};
            camera.viewPort.size = {app->width, app->height};

            gear::Transform3 transform;
            transform.position = {0, 0, 5};
            transform.scale = 1;
            transform.orientation = glm::quatLookAt(glm::normalize(glm::vec3{0,0,-1}), glm::vec3{0, 1, 0});
            cam = cmd.createEntity(transform, camera);
        }

        {
            gear::Transform3 transform{};
            transform.position = {0,0,0};
            transform.scale = 1;
            transform.orientation = glm::quatLookAt(glm::vec3{0,0,-1}, glm::vec3{0,1,0});

            gear::MeshInstance meshInstance{};
            meshInstance.mesh = assets->getMesh("SciFiHelmet");
            meshInstance.shader = assets->getShader("defaultShd");

            mesh = cmd.createEntity(transform, meshInstance);
        }

        {
            gear::Transform3 transform{};
            transform.position = {5,-2,0};
            transform.scale = 75;
            transform.orientation = glm::quatLookAt(glm::vec3{0,0,1}, glm::vec3{0,1,0});

            gear::MeshInstance meshInstance{};
            meshInstance.mesh = assets->getMesh("pCube49");
            meshInstance.shader = assets->getShader("defaultShd");

            mesh = cmd.createEntity(transform, meshInstance);
        }

        gear::ecs::executeCommandBuffer(cmd, world);
    }

    void update() override {
        g3d->clearBuffer({0,0,0,1}, 1);
        g3d->renderScene(world);

        {
            static float yaw = 0;
            auto[ct] = cam.get<gear::Transform3>();
            if (application->getInputState().keyDown(gear::KEYS::W)) {
                ct.position += ct.orientation * glm::vec3(0, 0, -1) * 0.05f;
            }
            if (application->getInputState().keyDown(gear::KEYS::S)) {
                ct.position += ct.orientation * glm::vec3(0, 0, 1) * 0.05f;
            }
            if (application->getInputState().keyDown(gear::KEYS::A)) {
                ct.position += ct.orientation * glm::vec3(-1, 0, 0) * 0.05f;
            }
            if (application->getInputState().keyDown(gear::KEYS::D)) {
                ct.position += ct.orientation * glm::vec3(1, 0, 0) * 0.05f;
            }
            if (application->getInputState().keyDown(gear::KEYS::Q)) {
                yaw += 0.01f;
            }
            if (application->getInputState().keyDown(gear::KEYS::E)) {
                yaw -= 0.01f;
            }
            ct.orientation = glm::quat(glm::vec3(0, yaw, 0));
        }
        if (false){
            auto [t] = mesh.get<gear::Transform3>();
            static float yaw = 0;
            yaw += 0.002f;
            t.orientation = glm::quat(glm::vec3(0, yaw, 0));
        }
    }

    void end() override {
        assets.reset();
        delete g3d;
    }

private:

    gear::G3DInstance* g3d;
    std::optional<gear::AssetRegistry> assets;
    gear::ecs::Registry world;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};

    gear::ecs::EntityRef cam;
    gear::ecs::EntityRef mesh;

    gear::Application* application;
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