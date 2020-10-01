cmake_minimum_required(VERSION "3.18.0")

project("moon-buggy-2d"
  LANGUAGES CXX
  VERSION "1.0.0"
  DESCRIPTION "A 2D implementation of the classic Moon Buggy terminal game"
)

include("FetchContent")

### Godot

FetchContent_Declare("Godot" GIT_REPOSITORY https://github.com/fmorgner/cmake-godot-cpp.git)
FetchContent_MakeAvailable("Godot")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_BINARY_DIR}/_deps/godot-src/Modules")
include("Godot")

### Conan

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()
list(APPEND CMAKE_MODULE_PATH "${PROJECT_BINARY_DIR}")
include("conan")

conan_add_remote(NAME "arknet" URL "https://conan.arknet.ch/artifactory/api/conan/public")
conan_cmake_run(REQUIRES
  "godot-cpp/3.2"
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD "missing"
  NO_OUTPUT_DIRS
  SETTINGS "os=${CMAKE_SYSTEM_NAME}"
  OUTPUT_QUIET
)

### Clang-tidy

find_program(CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy"
)

### Scripts

add_library("scripts" SHARED
  "source/scripts.cpp"
  "source/Buggy.cpp"
  "source/Game.cpp"
  "source/Level.cpp"
  "source/LevelGenerator.cpp"
  "source/MainMenu.cpp"
  "source/Map.cpp"
  "source/ScrollCamera.cpp"
)

target_include_directories("scripts" PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries("scripts" PRIVATE
  "CONAN_PKG::godot-cpp"
)

target_compile_options("scripts" PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-pedantic-errors>
)

set_target_properties("scripts" PROPERTIES
  CXX_STANDARD "20"
  CXX_STANDARD_REQUIRED YES
  INTERPROCEDURAL_OPTIMIZATION ON
  LINK_FLAGS_MINSIZEREL "-s"
  GODOT_BINARY_INSTALL_DIR "godot/bin"
)

if(NOT CMAKE_CROSSCOMPILING)
  set_target_properties("scripts" PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
  )
endif()

godot_register_library("scripts")
godot_register_class("scripts" "Buggy")
godot_register_class("scripts" "Game")
godot_register_class("scripts" "LevelGenerator")
godot_register_class("scripts" "MainMenu")
godot_register_class("scripts" "Map")
godot_register_class("scripts" "ScrollCamera")
