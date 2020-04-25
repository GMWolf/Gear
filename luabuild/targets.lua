---
--- Created by felix.
--- DateTime: 20/04/2020 18:04
---

local ninja = require("luabuild.ninja");

local M = {};

M.targets = {}

CURRENT_DIRECTORY = "";

function M.subfile(path, dir)
    --relative to current target dir
    if (dir) then
        dir = dir.."/";
    else
        dir = path:match("(.*/)") or "";
    end
    local current_dir = CURRENT_DIRECTORY;
    CURRENT_DIRECTORY = CURRENT_DIRECTORY..dir;
    local t = dofile(current_dir..path);
    CURRENT_DIRECTORY = current_dir;
    return t;
end


local function dedup(array)
    local hash = {}
    local res = {}

    for _,v in ipairs(array) do
        if (not hash[v]) then
            res[#res+1] = v -- you could print here instead of saving to result table if you wanted
            hash[v] = true
        end
    end

    return res;
end

---
--- Returns a lib table, and a list of build steps for that rule
--- lib takes form {name, sources}
---
function M.library(libDef)

    local oFiles = {};
    local includeDirs = {};
    local includeDirsExport = {};
    local archives = {};
    local linkArgs = libDef.linkArgs or {};
    local ideps = {};

    local arguments = {};

    local rule = libDef.rule or "CXX_COMPILER";

    if (libDef.include_directories) then
        if (libDef.include_directories.public) then
            for i, dir in ipairs(libDef.include_directories.public) do
                table.insert(includeDirs, "../"..CURRENT_DIRECTORY..dir);
                table.insert(includeDirsExport, "../"..CURRENT_DIRECTORY..dir);
            end
        end
        if (libDef.include_directories.generated_public) then
            for i, dir in ipairs(libDef.include_directories.generated_public) do
                table.insert(includeDirs, dir);
                table.insert(includeDirsExport, dir);
            end
        end
    end

    if libDef.libs then
        if libDef.libs.private then
            for i, lib in ipairs(libDef.libs.public) do
                for j, dir in ipairs(lib.includeDirs) do
                    table.insert(includeDirs, dir);
                end
                for j, archive in ipairs(lib.archives) do
                    table.insert(archives, archive);
                end
                for j, arg in ipairs(lib.linkArgs) do
                    table.insert(linkArgs, arg);
                end
                for j, dep in ipairs(lib.ideps) do
                    table.insert(ideps, dep);
                end
            end
        end


        if libDef.libs.public then
            for i, lib in ipairs(libDef.libs.public) do
                for j, dir in ipairs(lib.includeDirs) do
                    table.insert(includeDirs, dir);
                    table.insert(includeDirsExport, dir);
                end
                for j, archive in ipairs(lib.archives) do
                    table.insert(archives, archive);
                end
                for j, arg in ipairs(lib.linkArgs) do
                    table.insert(linkArgs, arg);
                end
                for j, dep in ipairs(lib.ideps) do
                    table.insert(ideps, dep);
                end
            end
        end
    end

    archives = dedup(archives);
    linkArgs = dedup(linkArgs);
    ideps = dedup(ideps);

    for i, dir in ipairs(includeDirs) do
        table.insert(arguments, "-I"..dir);
    end

    if (libDef.definitions) then
        for name, value in pairs(libDef.definitions) do
            print(name.."="..value);
            table.insert(arguments, "-D"..name.."="..value);
        end
    end

    if libDef.sources then
        for i, relsrc in ipairs(libDef.sources) do
            local src = CURRENT_DIRECTORY..relsrc;
            local obj = src..".o";
            ninja.writeStep( {
                rule = rule,
                inputs = "../"..src,
                outputs = obj,
                variables = {
                    args = table.concat(arguments, " ");
                };
                implicit_dependencies = ideps;
            });
            table.insert(oFiles, obj);
        end

        ninja.writeStep({
            rule = "CXX_ARCHIVE",
            inputs = table.concat(oFiles, " "),
            outputs = CURRENT_DIRECTORY.."lib"..libDef.name..".a";
        });

        table.insert(archives, 1, CURRENT_DIRECTORY.."lib"..libDef.name..".a");
    end

    return {
        name = libDef.name;
        archives = archives; --archive to link to
        includeDirs = includeDirsExport; --includes to include
        linkArgs = linkArgs;
        ideps = ideps;
    }

end


function M.executable(exeDef)

    local lib = M.library({
       name = exeDef.name;
       sources = exeDef.sources;
       libs = exeDef.libs;
       include_directories = exeDef.include_directories;
    });

    ninja.writeStep({
        rule = "CXX_LINKER",
        inputs = table.concat(lib.archives, " "),
        outputs = CURRENT_DIRECTORY..exeDef.name;
        variables = {
            args = table.concat(lib.linkArgs, " ");
        };
    });

    return {
        exe = CURRENT_DIRECTORY..exeDef.name;
    }

end



return M;