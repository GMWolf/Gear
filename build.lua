---
--- Created by felix.
--- DateTime: 20/04/2020 18:08
---

ninja = require("luabuild.ninja");
c_rules = require("luabuild.crules");
targets = require("luabuild.targets");

imgui_lib = targets.subfile("imgui.lua");
glad_lib = targets.subfile("glad.lua");
stb_lib = targets.subfile("stb.lua");
json_lib = targets.subfile("json.lua");
tinyxml2_lib = targets.subfile("tinyxml2.lua");
base64_lib = targets.subfile("base64.lua");

gear_lib = targets.subfile("gear/gear.lua");

targets.subfile("examples/shmup/shmup.lua");