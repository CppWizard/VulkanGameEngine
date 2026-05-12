add_executable(Editor
    src/EditorMain.cpp
)

target_link_libraries(Editor
    PRIVATE
        VulkanGameEngine::Engine
        imgui_backend
)

set_project_options(Editor)
set_project_warnings(Editor)

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)