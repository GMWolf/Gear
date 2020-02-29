
set(gear_tools_shader_pack_internal_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

function(add_shader name vertex fragment)
    file(READ ${vertex} vertexText)
    file(READ ${fragment} fragmentText)
    configure_file(${gear_tools_shader_pack_internal_dir}/template.glsl.in ${name})

    add_custom_target(${name} DEPENDS ${name})

    set_target_properties(${name} PROPERTIES
            GEAR_ASSET_PATH "${name}"
            GEAR_ASSET_TYPE "shader")
endfunction()