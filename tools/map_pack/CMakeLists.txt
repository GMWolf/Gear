
add_executable(map_packer map_packer.cpp)
target_link_libraries(map_packer PUBLIC tinyxml2 cpp-base64 libzstd_static gear-assets-fbs)
target_include_directories(map_packer PUBLIC ../../)

function(add_map input)
    add_custom_command(
            COMMAND map_packer ARGS ${CMAKE_CURRENT_LIST_DIR}/${input} ${input}.bin
            OUTPUT ${input}.bin
            DEPENDS "${input}" map_packer
            USES_TERMINAL
    )

    set(ADD_MAP_OUT_FILES ${input}.bin PARENT_SCOPE)
    set(ADD_MAP_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()

