cmake_minimum_required(VERSION 3.24)

project(VulkanGameEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_EDITOR "Build editor application" ON)
option(BUILD_GAME "Build game application" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ProjectOptions)
include(CompilerWarnings)
include(Shaders)

find_package(Vulkan REQUIRED)

add_subdirectory(external)
add_subdirectory(engine)
add_subdirectory(shaders)

if(BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if(BUILD_GAME)
    add_subdirectory(game)
endif()