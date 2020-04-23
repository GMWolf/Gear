---
--- Created by felix.
--- DateTime: 20/04/2020 23:03
---

local target = require("luabuild.targets");

local imgui_core = target.library {
    name = "imgui_core";
    sources = {
        "imgui.cpp",
        "imgui_widgets.cpp",
        "imgui_draw.cpp",
        "imgui_demo.cpp",
    };
    include_directories = {
        public = {"../imgui"}
    }
}

local imgui_glfw = target.library {
    name = "imgui_glfw";
    sources = {
        "examples/imgui_impl_glfw.cpp"
    };
    include_directories = {
        public = {"examples"}
    };
    libs = {
        public = { imgui_core }
    }
}

local imgui_ogl3 = target.library {
    name = "imgui_ogl3";
    sources = {
        "examples/imgui_impl_opengl3.cpp"
    };
    include_directories = {
        public = {"examples"}
    };
    libs = {
        public = { imgui_core, glad_lib }
    };
    definitions = {
        IMGUI_IMPL_OPENGL_LOADER_GLAD = 1;
    }
}

return target.library {
    name = "imgui";
    libs = {
        public = {imgui_core, imgui_glfw, imgui_ogl3 }
    }
}