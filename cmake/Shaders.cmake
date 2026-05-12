function(compile_shader TARGET_NAME SHADER_FILE)
    find_program(GLSLC glslc REQUIRED)

    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    set(SPIRV_FILE "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${GLSLC} ${SHADER_FILE} -o ${SPIRV_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling shader ${FILE_NAME}"
        VERBATIM
    )

    target_sources(${TARGET_NAME} PRIVATE ${SPIRV_FILE})
endfunction()