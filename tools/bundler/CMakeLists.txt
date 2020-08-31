add_executable(bundler bundler.cpp)
target_link_libraries(bundler PUBLIC gear-assets-fbs)


function(add_bundle output deptarget)
    add_custom_command(
            COMMAND bundler ARGS ${output} ${ARGN}
            OUTPUT ${output}
            DEPENDS ${ARGN} bundler ${deptarget}
            USES_TERMINAL
    )
endfunction()