function(apply_project_options target scope)
    set(common_options
        -Wall
        -Wextra
        -Werror
        -Wno-implicit-fallthrough
        -Wno-unused-parameter
        -Wunused-variable
        -Wno-missing-field-initializers
        -Wnull-dereference
        -ffast-math
    )

    foreach(option IN LISTS common_options)
        target_compile_options(${target} ${scope}
            $<$<CXX_COMPILER_ID:GNU>:${option}>
            $<$<CXX_COMPILER_ID:Clang>:${option}>
        )
    endforeach()

    set(gcc_only_options
        -Wduplicated-cond
        -Wlogical-op
        -Wrestrict
        -finput-charset=UTF-8
        -fexec-charset=UTF-8
    )

    foreach(option IN LISTS gcc_only_options)
        target_compile_options(${target} ${scope}
            $<$<CXX_COMPILER_ID:GNU>:${option}>
        )
    endforeach()

    set(gcc_14_options
        -fdiagnostics-all-candidates
    )

    foreach(option IN LISTS gcc_14_options)
        target_compile_options(${target} ${scope}
            $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,14>>:${option}>
        )
    endforeach()

    set(msvc_options
        /Zc:preprocessor
    )

    foreach(option IN LISTS msvc_options)
        target_compile_options(${target} ${scope}
            $<$<CXX_COMPILER_ID:MSVC>:${option}>
        )
    endforeach()
endfunction()
