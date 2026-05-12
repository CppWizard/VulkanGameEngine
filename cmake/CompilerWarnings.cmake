function(set_project_warnings target)
    if(NOT ENABLE_WARNINGS)
        return()
    endif()

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX-)
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
        )
    endif()
endfunction()