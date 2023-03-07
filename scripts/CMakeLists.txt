cmake_minimum_required(VERSION "3.25.0")

project("Moon Buggy 2D"
    LANGUAGES CXX
    VERSION "${MOON_BUGGY_2D_VERSION}"
    DESCRIPTION "${MOON_BUGGY_2D_DESCRIPTION}"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MOON_BUGGY_2D_BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MOON_BUGGY_2D_BIN_DIR}")

# External Dependencies

include("FetchContent")

FetchContent_Declare("godot-cpp"
    GIT_REPOSITORY "https://github.com/godotengine/godot-cpp.git"
    GIT_TAG "godot-4.0-stable"
    # OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable("godot-cpp")

# Scripts

add_library("scripts" SHARED "scripts.cpp")

target_compile_options("scripts" PUBLIC
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-pedantic-errors>"
)

target_link_libraries("scripts" PUBLIC
    "godot::cpp"
)

target_include_directories("scripts" PUBLIC
    "${PROJECT_SOURCE_DIR}"
)

set_target_properties("scripts" PROPERTIES
    OUTPUT_NAME "scripts.${CMAKE_SYSTEM_NAME}.${CMAKE_BUILD_TYPE}"
)