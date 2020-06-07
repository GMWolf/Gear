---
--- Created by felix.
--- DateTime: 20/04/2020 18:08
---

ninja = require("luabuild.ninja");

c_rules = require("luabuild.crules");
targets = require("luabuild.targets");

targets.subfile("deps/deps.lua")


gear_lib = targets.subfile("gear/gear.lua");

targets.subfile("tools/texture_pack/texture_packer.lua");
targets.subfile("tools/shader_pack/shader_pack.lua");
targets.subfile("tools/font_pack/font_pack.lua");
targets.subfile("tools/map_pack/map_pack.lua");
targets.subfile("tools/tileset_pack/tileset_pack.lua")

targets.subfile("examples/shmup/shmup.lua");
targets.subfile("examples/breakout/breakout.lua");

ninja.close();

os.execute("ninja -C build -t compdb CXX_COMPILER > compile_commands.json");
os.execute( "ninja -C build");