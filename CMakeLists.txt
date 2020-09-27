cmake_minimum_required(VERSION "3.18.0")

project("moon-buggy-2d"
  LANGUAGES CXX
  VERSION "1.0.0"
  DESCRIPTION "A 2D implementation of the classic Moon Buggy terminal game"
)

include("FetchContent")

FetchContent_Declare("Godot" GIT_REPOSITORY https://github.com/fmorgner/cmake-godot-cpp.git)
FetchContent_MakeAvailable("Godot")

list(APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake/Modules"
  "${PROJECT_BINARY_DIR}/_deps/godot-src/Modules"
)

include("Conan")
include("Godot")

conan_check(REQUIRED)
conan_add_remote(NAME "arknet" URL "https://conan.arknet.ch/artifactory/api/conan/public")
conan_cmake_run(CONANFILE "conanfile.txt"
  OUTPUT_QUIET
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD "missing"
  NO_OUTPUT_DIRS
  SETTINGS "os=${CMAKE_SYSTEM_NAME}"
)

add_library("scripts" SHARED
  "source/scripts.cpp"
  "source/Buggy.cpp"
  "source/Game.cpp"
  "source/GameBoundary.cpp"
  "source/LevelGenerator.cpp"
  "source/Map.cpp"
  "source/ScrollCamera.cpp"
)

target_include_directories("scripts" PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries("scripts" PRIVATE
  "CONAN_PKG::godot-cpp"
)

set_target_properties("scripts" PROPERTIES
  CXX_STANDARD "17"
  CXX_STANDARD_REQUIRED YES
  INTERPROCEDURAL_OPTIMIZATION ON
  LINK_FLAGS_MINSIZEREL "-s"
  GODOT_BINARY_INSTALL_DIR "godot/bin"
)

godot_register_library("scripts")
godot_register_class("scripts" "Buggy")
godot_register_class("scripts" "Game")
godot_register_class("scripts" "GameBoundary")
godot_register_class("scripts" "LevelGenerator")
godot_register_class("scripts" "Map")
godot_register_class("scripts" "ScrollCamera")