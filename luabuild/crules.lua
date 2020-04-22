---
--- Created by felix.
--- DateTime: 20/04/2020 18:22
---

ninja = require("luabuild.ninja");

local M = {
    CXX_COMPILER = {
        name = "CXX_COMPILER";
        command = "clang++ -std=c++17 -c $in -o $out $args";
    };

    C_COMPILER = {
        name = "C_COMPILER";
        command = "clang -std=c99 -c $in -o $out $args";
    };

    CXX_LINKER = {
        name = "CXX_LINKER";
        command = "clang++ -o $out $in $args";
    };

    CXX_ARCHIVE = {
        name = "CXX_ARCHIVE";
        command = "ar rcs $out $in";
    };

}

for name, rule in pairs(M) do
    ninja.writeRule(rule);
end

return M;