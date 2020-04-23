---
--- Created by felix.
--- DateTime: 20/04/2020 23:23
---

local targets = require("luabuild.targets");

return targets.library {

    name = "nlohmann_json";
    include_directories = {
        public = {"include"}
    }

}