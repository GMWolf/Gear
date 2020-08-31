
add_executable(tileset_pack tileset_pack.cpp)
target_link_libraries(tileset_pack PUBLIC stb tinyxml2 gear_tools_texture gear-assets-fbs)
target_include_directories(tileset_pack PUBLIC ../../)

function(add_tileset_pack input)
    add_custom_command(
            COMMAND tileset_pack ARGS ${CMAKE_CURRENT_LIST_DIR}/${input} ${input}.bin
            OUTPUT ${input}.bin
            DEPENDS "${input}" tileset_pack
            USES_TERMINAL
    )

    set(ADD_TILESET_OUT_FILES ${input}.bin PARENT_SCOPE)
    set(ADD_TILESET_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()