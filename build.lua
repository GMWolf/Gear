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

targets.subfile("examples/shmup/shmup.lua");

ninja.close();

os.execute("ninja -C build -t compdb CXX_COMPILER C_COMPILER CXX_LINKER CXX_ARCHIVE > ../compile_commands.json");
os.execute( "ninja -C build");