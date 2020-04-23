---
--- Created by felix.
--- DateTime: 20/04/2020 23:21
---

local target = require("luabuild.targets");

return target.library {
    name = "stb";
    sources = {
        "stb_impl.cpp"
    };
    include_directories = {
        public = {"stb"}
    };
};