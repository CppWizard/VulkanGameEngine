add_executable(Game
    src/GameMain.cpp
)

target_link_libraries(Game
    PRIVATE
        VulkanGameEngine::Engine
)

set_project_options(Game)
set_project_warnings(Game)

set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)