cmake_minimum_required(VERSION 3.14)

project(Minutea LANGUAGES CXX)

include(FetchContent)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Minutea)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(FETCHCONTENT_QUIET OFF)

set(wxBUILD_SHARED OFF)
FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG        v3.2.6
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(wxWidgets)

file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.hpp")

add_executable(Minutea ${SOURCES})
target_link_libraries(Minutea PRIVATE wxcore wxbase)
target_precompile_headers(Minutea PRIVATE src/pch.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SOURCES})

if (WIN32)
    target_link_options(Minutea PRIVATE "/SUBSYSTEM:WINDOWS")
endif()

if (MSVC)
    target_compile_options(Minutea PRIVATE /MP)
endif()