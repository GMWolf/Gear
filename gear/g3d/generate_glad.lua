local api = "gl=4.6";
local spec = "gl";
local extensions = {
    "GL_KHR_debug"
};

os.execute("glad --profile core --api=\""..api.."\" --generator=\"c-debug\" --spec=\"gl\" --extensions=\""..table.concat(extensions, ",").."\" --out-path glad");