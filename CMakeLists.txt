cmake_minimum_required(VERSION 3.14)

project(another-day LANGUAGES C CXX)

include(FetchContent)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT another-day)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FETCHCONTENT_QUIET OFF)

set(wxBUILD_SHARED OFF)
FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG        v3.2.6
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)
FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2025/sqlite-amalgamation-3490000.zip
    URL_HASH SHA256=cb6851ebad74913672014c20f642bbd7883552c4747780583a54ee1cd493f13b
)

FetchContent_MakeAvailable(wxWidgets sqlite3)

add_library(sqlite3 STATIC
    ${sqlite3_SOURCE_DIR}/sqlite3.c
    ${sqlite3_SOURCE_DIR}/sqlite3.h
)
target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.hpp")

set(TARGET "another-day")
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE wxcore wxbase sqlite3)
target_precompile_headers(${TARGET} PRIVATE src/pch.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SOURCES})

if (WIN32)
    target_link_options(${TARGET} PRIVATE "/SUBSYSTEM:WINDOWS")
endif()

if (MSVC)
    target_compile_options(${TARGET} PRIVATE /MP)
endif()