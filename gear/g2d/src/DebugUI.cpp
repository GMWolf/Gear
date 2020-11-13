//
// Created by felix on 04/03/2020.
//


#include <DebugUI.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

bool gear::ui::demoWindowOpen = true;

void gear::ui::initialize(GLFWwindow *window) {

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

}

void gear::ui::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void gear::ui::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void gear::ui::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gear::ui::perfWindow(gear::ui::PerfData &data) {

    if (data.plot.size() < data.maxCount) {
        data.plot.push_back(data.frameTime);
    } else {
        data.plot.erase(data.plot.begin());
        data.plot.push_back(data.frameTime);
    }

    ImGui::Begin("FPS");
    ImGui::Text("%03.1f", 1./data.frameTime);
    ImGui::PlotHistogram("frame time", data.plot.data(), data.plot.size());
    ImGui::End();
}
