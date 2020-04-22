---
--- Created by felix.
--- DateTime: 20/04/2020 18:08
---

ninja = require("luabuild.ninja");
c_rules = require("luabuild.crules");
targets = require("luabuild.targets");

glad_lib = targets.subfile("glad.lua", "deps/glad");
imgui_lib = targets.subfile("imgui.lua", "deps/imgui");
stb_lib = targets.subfile("stb.lua");
json_lib = targets.subfile("json.lua", "deps/json");
tinyxml2_lib = targets.subfile("tinyxml2.lua", "deps/tinyxml2");
base64_lib = targets.subfile("base64.lua", "deps/cpp-base64");
x11 = true;
glfw_lib = targets.subfile("glfw.lua", "deps/glfw");

gear_lib = targets.subfile("gear/gear.lua");

targets.subfile("examples/shmup/shmup.lua");