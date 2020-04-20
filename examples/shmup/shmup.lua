---
--- Created by felix.
--- DateTime: 20/04/2020 23:33
---

local targets = require("luabuild.targets");

targets.executable {

    name = "shmup";
    sources = {"src/main.cpp", "src/Collisions.cpp"};
    libs = {
        public = {
            gear_lib
        }
    }

}