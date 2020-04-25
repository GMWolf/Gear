

targets = require("luabuild.targets");
path = require("path");

local compiler_source = {
    "src/idl_parser.cpp",
    "src/idl_gen_text.cpp",
    "src/reflection.cpp",
    "src/util.cpp",
    "src/idl_gen_cpp.cpp",
    "src/idl_gen_csharp.cpp",
    "src/idl_gen_dart.cpp",
    "src/idl_gen_kotlin.cpp",
    "src/idl_gen_go.cpp",
    "src/idl_gen_java.cpp",
    "src/idl_gen_js_ts.cpp",
    "src/idl_gen_php.cpp",
    "src/idl_gen_python.cpp",
    "src/idl_gen_lobster.cpp",
    "src/idl_gen_lua.cpp",
    "src/idl_gen_rust.cpp",
    "src/idl_gen_fbs.cpp",
    "src/idl_gen_grpc.cpp",
    "src/idl_gen_json_schema.cpp",
    "src/idl_gen_swift.cpp",
    "src/flatc.cpp",
    "src/flatc_main.cpp",
    "src/code_generators.cpp",
    "grpc/src/compiler/cpp_generator.cc",
    "grpc/src/compiler/go_generator.cc",
    "grpc/src/compiler/java_generator.cc",
    "grpc/src/compiler/python_generator.cc",
    "grpc/src/compiler/swift_generator.cc"
};



local flatc_exe = targets.executable({
    name = "flatc";
    sources = compiler_source;
    include_directories = {
        public = { "include", "grpc" };
    };
});

ninja.writeRule({
   name = "FLATC";
   command = flatc_exe.exe.." --cpp -o $path $in";
});


flatbuffers_lib = targets.library( {
   name = "flatbuffers";
   include_directories = {
       public = {"include"};
   }
});


function flatc(file)

    local dir = CURRENT_DIRECTORY..file:match("(.-)([^\\/]-%.?([^%.\\/]*))$");
    print(dir);
    ninja.writeStep({
       rule = "FLATC";
       outputs = dir..file.."_generated.h";
       inputs = "../"..CURRENT_DIRECTORY..file;
       implicit_dependencies = {flatc_exe.exe};
       variables = {
           path = dir; -- get the directory
       }
    });

    return targets.library({
        name = "file";
        include_directories = {
            generated_public = {
                CURRENT_DIRECTORY;
            }
        };
        libs = {
            public = {
                flatbuffers_lib;
            }
        };
        ideps = {
            CURRENT_DIRECTORY..file.."_generated.h";
        }
    });
end