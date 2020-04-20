---
--- Created by felix.
--- DateTime: 20/04/2020 23:30
---

local target = require("luabuild.targets");

return target.library {
    name = "cpp_base64";
    sources = { "deps/cpp-base64/base64.cpp"};
    include_directories = {
        public = { "deps/cpp-base64" }
    }
}