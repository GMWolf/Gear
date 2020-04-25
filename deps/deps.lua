---
--- Created by felix.
--- DateTime: 22/04/2020 23:01
---

local targets = require("luabuild.targets");

glad_lib = targets.subfile("glad.lua", "glad");
imgui_lib = targets.subfile("imgui.lua", "imgui");
stb_lib = targets.subfile("stb.lua");
json_lib = targets.subfile("json.lua", "json");
tinyxml2_lib = targets.subfile("tinyxml2.lua", "tinyxml2");
base64_lib = targets.subfile("base64.lua", "cpp-base64");
x11 = true;
glfw_lib = targets.subfile("glfw.lua", "glfw");

lyra_lib = targets.subfile("lyra.lua", "Lyra");
yaml_lib = targets.subfile("yaml.lua", "yaml-cpp");

targets.subfile("flatbuffers.lua", "flatbuffers");