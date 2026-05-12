function(set_project_options target)
    target_compile_features(${target} PUBLIC cxx_std_20)

    if(MSVC)
        target_compile_options(${target} PRIVATE /permissive- /Zc:preprocessor)
    endif()

    target_compile_definitions(${target}
        PRIVATE
            $<$<CONFIG:Debug>:ENGINE_DEBUG>
            $<$<CONFIG:Release>:ENGINE_RELEASE>
    )
endfunction()