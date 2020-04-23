---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by felix.
--- DateTime: 23/04/2020 00:43
---

local vertFile = io.open(arg[1]);

local vertexText = vertFile:read("*all");

local fragFile = io.open(arg[2]);

local fragText = fragFile:read("*all");

local outFile = io.open(arg[3], 'w');

outFile:write("#ifdef VERTEX_SHADER\n");
outFile:write(vertexText);
outFile:write("\n#endif\n\n#ifdef FRAGMENT_SHADER\n");
outFile:write(fragText);
outFile:write("\n#endif\n");