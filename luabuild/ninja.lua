---
--- Created by felix.
--- DateTime: 20/04/2020 18:05
---

local M = {};

local file = io.open("build/build.ninja", "w");


function M.close()
    file:close();
end


function M.writeRule( rule)
    file:write("rule ", rule.name, "\n");
    file:write("  command = ", rule.command, "\n");
    if (rule.generator) then
        file:write("  generator = 1\n");
    end

    file:write("\n\n");
end


function M.writeStep(step)
    file:write("build ", step.outputs, " : ", step.rule, " ", step.inputs);

    if step.implicit_dependencies then
        file:write(" | ", table.concat(step.implicit_dependencies, " "));
    end

    file:write("\n");

    if step.variables then
        for k, v in pairs(step.variables) do
            file:write("  ", k, " = ", v, "\n");
        end
    end
    file:write("\n\n");
end


return M;