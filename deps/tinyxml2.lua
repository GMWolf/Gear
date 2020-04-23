---
--- Created by felix.
--- DateTime: 20/04/2020 23:26
---

local target = require("luabuild.targets");

return target.library {
    name = "tinyxml2";
    sources = { "tinyxml2.cpp"};
    include_directories = {
        public = { "../tinyxml2" }
    }
}