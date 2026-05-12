file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)

add_library(Engine STATIC
    ${ENGINE_HEADERS}
    ${ENGINE_SOURCES}
 "include/Engine/Core/Timer.cpp")

add_library(VulkanGameEngine::Engine ALIAS Engine)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Engine
    PUBLIC
        Vulkan::Vulkan
        glfw
        glm::glm
)

set_project_options(Engine)
set_project_warnings(Engine)

target_compile_definitions(Engine
    PUBLIC
        GLFW_INCLUDE_NONE
)