---
--- Created by felix.
--- DateTime: 20/04/2020 23:03
---

local target = require("luabuild.targets");

local imgui_core = target.library {
    name = "imgui_core";
    sources = {
        "deps/imgui/imgui.cpp",
        "deps/imgui/imgui_widgets.cpp",
        "deps/imgui/imgui_draw.cpp",
        "deps/imgui/imgui_demo.cpp",
    };
    include_directories = {
        public = {"deps/imgui"}
    }
}

local imgui_glfw = target.library {
    name = "imgui_glfw";
    sources = {
        "deps/imgui/examples/imgui_impl_glfw.cpp"
    };
    include_directories = {
        public = {"deps/imgui/examples"}
    };
    libs = {
        public = { imgui_core }
    }
}

local imgui_ogl3 = target.library {
    name = "imgui_ogl3";
    sources = {
        "deps/imgui/examples/imgui_impl_opengl3.cpp"
    };
    include_directories = {
        public = {"deps/imgui/examples"}
    };
    libs = {
        public = { imgui_core }
    }
}

return target.library {
    name = "imgui";
    libs = {
        public = {imgui_core, imgui_glfw, imgui_ogl3 }
    }
}