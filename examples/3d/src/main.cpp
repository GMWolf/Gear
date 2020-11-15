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
#include <glm/gtc/random.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <Remotery.h>
#include <iostream>

class Game : public gear::ApplicationAdapter {
public:

    void init(gear::Application *app) override {
        application = app;

        {
            _rmt_Settings()->reuse_open_port = true;
            auto error = rmt_CreateGlobalInstance(&rmt);
            if (error != RMT_ERROR_NONE) {
                std::cerr << "Error launching remotery " << error << "\n";
            }
        }

        g3d = new gear::G3DInstance();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(app->window, true);
        ImGui_ImplOpenGL3_Init();

        assets.emplace();

        assets->loadBundle("assets.bin");
        assets->loadBundle("assets/models/SciFiHelmet/SciFiHelmet.bundle");
        assets->loadBundle("assets/models/Corset/Corset.bundle");
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

        for(int i = 0; i < 50; i++)
        {
            gear::Transform3 transform{};
            auto p = glm::diskRand(25.0f);
            transform.position = {p.x,0,p.y};
            transform.scale = 1;
            p = glm::diskRand(1.0f);
            transform.orientation = glm::quatLookAt(glm::normalize(glm::vec3(p.x, 0, p.y)), glm::vec3{0,1,0});

            gear::MeshInstance meshInstance{};
            meshInstance.mesh = assets->getMesh("SciFiHelmet");
            meshInstance.shader = assets->getShader("defaultShd");

            mesh = cmd.createEntity(transform, meshInstance);
        }

        for(int i = 0; i < 50; i++)
        {
            gear::Transform3 transform{};
            auto p = glm::diskRand(25.0f);
            transform.position = {p.x,-2,p.y};
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

        rmt_ScopedCPUSample(GameUpdate, 0);
        rmt_ScopedOpenGLSample(GameRender);

        g3d->clearBuffer({0,0,0,1}, 1);
        g3d->renderScene(world);
        drawUI();
        {
            rmt_ScopedCPUSample(CameraMovement, 0);


            float dt = (float)application->frameTime;
            static float yaw = 0;
            auto[ct] = cam.get<gear::Transform3>();
            if (application->getInputState().keyDown(gear::KEYS::W)) {
                ct.position += ct.orientation * glm::vec3(0, 0, -1) * dt * 4.0f;
            }
            if (application->getInputState().keyDown(gear::KEYS::S)) {
                ct.position += ct.orientation * glm::vec3(0, 0, 1) * dt * 4.0f;
            }
            if (application->getInputState().keyDown(gear::KEYS::A)) {
                ct.position += ct.orientation * glm::vec3(-1, 0, 0) * dt * 4.0f;
            }
            if (application->getInputState().keyDown(gear::KEYS::D)) {
                ct.position += ct.orientation * glm::vec3(1, 0, 0) * dt * 4.0f;
            }
            if (application->getInputState().keyDown(gear::KEYS::Q)) {
                yaw += dt;
            }
            if (application->getInputState().keyDown(gear::KEYS::E)) {
                yaw -= dt;
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
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        assets.reset();
        delete g3d;

        rmt_DestroyGlobalInstance(rmt);

    }

    void drawUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("FPS");
        ImGui::Text("%03.1f", 1./application->frameTime);
        //ImGui::PlotHistogram("frame time", data.plot.data(), data.plot.size());
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:

    gear::G3DInstance* g3d;
    std::optional<gear::AssetRegistry> assets;
    gear::ecs::Registry world;
    gear::ecs::EntityPool pool;
    gear::ecs::CommandBuffer cmd{pool, 256'000'000};

    gear::ecs::EntityRef cam;
    gear::ecs::EntityRef mesh;

    Remotery* rmt;

    gear::Application* application;
};

int main() {

    gear::AppConfig config {
            .width = 1920,
            .height = 1080,
            .title ="3d",
            .gapi = gear::g3dGetGapi()
    };

    Game game;
    gear::Application app(config);
    app.run(game);

    return 0;
}