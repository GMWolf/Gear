---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by felix.
--- DateTime: 20/04/2020 22:29
---

local target = require("luabuild.targets");

return target.library {
    name = "glad";
    sources = {
        "src/glad.cpp"
    };
    include_directories = {
        public = {"include"}
    }
}