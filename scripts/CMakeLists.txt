cmake_minimum_required(VERSION "3.25.0")

project("Moon Buggy 2D"
    LANGUAGES CXX
    VERSION "${MOON_BUGGY_2D_VERSION}"
    DESCRIPTION "${MOON_BUGGY_2D_DESCRIPTION}"
)

include("CheckIPOSupported")
check_ipo_supported(RESULT MB2D_IPO_IS_SUPPORTED)

# External Dependencies

find_package("godot-cpp")

# Scripts

add_library("scripts" SHARED "scripts.cpp")

target_compile_options("scripts" PUBLIC
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:-pedantic-errors>"
)

target_link_libraries("scripts" PUBLIC
    "godot-cpp::godot-cpp"
)

target_include_directories("scripts" PUBLIC
    "${PROJECT_SOURCE_DIR}"
)

set_target_properties("scripts" PROPERTIES
    OUTPUT_NAME "scripts.$<LOWER_CASE:$<PLATFORM_ID>>.$<LOWER_CASE:${CMAKE_SYSTEM_PROCESSOR}>.$<LOWER_CASE:$<CONFIG>>"
    PREFIX ""
    INTERPROCEDURAL_OPTIMIZATION ${MB2D_IPO_IS_SUPPORTED}
)
