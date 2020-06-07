add_executable(bundler bundler.cpp)
target_include_directories(bundler PUBLIC ../../)


function(add_bundle output deptarget)
    message("${ARGN}")
    add_custom_command(
            COMMAND bundler ARGS ${output} ${ARGN}
            OUTPUT ${output}
            DEPENDS ${ARGN} bundler ${deptarget}
            USES_TERMINAL
    )
endfunction()