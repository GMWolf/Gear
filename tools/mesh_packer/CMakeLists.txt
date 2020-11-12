
add_executable(mesh_packer mesh_packer.cpp)
target_link_libraries(mesh_packer PUBLIC tinygltf gear-assets-fbs stb meshoptimizer glm)

function(add_mesh input)
    add_custom_command(
            COMMAND mesh_packer ARGS ${CMAKE_CURRENT_LIST_DIR}/${input} ${input}.bin
            OUTPUT ${input}.bin
            DEPENDS "${input}" mesh_packer
            USES_TERMINAL
    )

    set(ADD_MESH_OUT_FILES ${input}.bin PARENT_SCOPE)
    set(ADD_MESH_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()

