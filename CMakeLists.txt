cmake_minimum_required(VERSION "3.13.0")

project("Moon Buggy 2D"
  LANGUAGES CXX
  VERSION "1.0.0"
  DESCRIPTION "A 2D implementation of the classic Moon Buggy terminal game"
)

include("CTest")
include("FetchContent")

### Conan

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")

if(CMAKE_CROSSCOMPILING)
  set(OS_SETTING SETTING "os=${CMAKE_SYSTEM_NAME}")
endif()

conan_cmake_run(CONANFILE "conanfile.txt"
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD "missing"
  ${OS_SETTING}
  OUTPUT_QUIET
)

### Boost SML

FetchContent_Declare("BoostSML"
  GIT_REPOSITORY "https://github.com/boost-ext/sml"
  GIT_TAG "v1.1.3"
)

FetchContent_Populate("BoostSML")

add_library("sml" INTERFACE)

target_include_directories("sml" SYSTEM INTERFACE
  $<BUILD_INTERFACE:${boostsml_SOURCE_DIR}/include>
)

### Godot

find_program(GODOT_EXE
  NAMES "godot" "org.godotengine.Godot"
  DOC "Godot engine executable"
)

### Clang-tidy

option(ENABLE_CLANG_TIDY "Enable linting using clang tidy" OFF)

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE
    NAMES "clang-tidy" "clang-tidy-10" "clang-tidy-9"
    DOC "Path to clang-tidy"
    HINTS "/bin" "/usr/bin" "/usr/local/bin" "/usr/local/opt/llvm/bin"
  )
endif()

### Scripts

add_library("scripts" SHARED
  "source/scripts.cpp"

  "source/core/Level.cpp"
  "source/core/LevelDescriptor.cpp"
  "source/core/Meteor.cpp"
  "source/core/MeteorSpawner.cpp"
  "source/core/ScrollCamera.cpp"
  "source/core/Space.cpp"
  "source/core/Theming.cpp"
  "source/game/Buggy.cpp"
  "source/game/Game.cpp"
  "source/game/LevelGenerator.cpp"
  "source/game/Map.cpp"
  "source/gui/BuggyCrashed.cpp"
  "source/gui/GUI.cpp"
  "source/gui/HUD.cpp"
  "source/gui/LevelComplete.cpp"
  "source/gui/MainMenu.cpp"
)

target_include_directories("scripts" PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries("scripts" PRIVATE
  "CONAN_PKG::godot-cpp"
  "sml"
)

target_compile_options("scripts" PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-pedantic-errors>
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:MSVC>:/experimental:external>
  $<$<CXX_COMPILER_ID:MSVC>:/external:anglebrackets>
  $<$<CXX_COMPILER_ID:MSVC>:/external:W0>
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_options("scripts" PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:--static>
    $<$<CXX_COMPILER_ID:GNU>:-Wl,--no-undefined>
    $<$<CXX_COMPILER_ID:GNU>:-static-libgcc>
    $<$<CXX_COMPILER_ID:GNU>:-static-libstdc++>
  )
endif()

set_target_properties("scripts" PROPERTIES
  CXX_STANDARD "20"
  CXX_STANDARD_REQUIRED YES
  INTERPROCEDURAL_OPTIMIZATION ON
  LINK_FLAGS_MINSIZEREL "-s"
  LINK_FLAGS_RELEASE "-s"
  PREFIX ""
)

add_custom_command(TARGET "scripts"
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:scripts> ${PROJECT_SOURCE_DIR}/godot/bin
  COMMENT "Make script library available to Godot"
)

if(NOT CMAKE_CROSSCOMPILING)
  set_target_properties("scripts" PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
  )
endif()

### GUT Tests

if(GODOT_EXE)
  add_test(NAME "GUTTests"
    COMMAND "${GODOT_EXE}"
    "--verbose"
    "--path" "${PROJECT_SOURCE_DIR}/godot"
    "-s" "addons/gut/gut_cmdln.gd"
  )
endif()

### Export

if(GODOT_EXE)
  set(GODOT_EXPORT_PLATFORM "${CMAKE_SYSTEM_NAME}")
  if(GODOT_EXPORT_PLATFORM STREQUAL "Darwin")
    set(GODOT_EXPORT_PLATFORM "macOS")
  endif()

  if(GODOT_EXPORT_PLATFORM STREQUAL "Linux")
    set(GODOT_EXPORT_EXTENSION "x86_64")
  elseif(GODOT_EXPORT_PLATFORM STREQUAL "macOS")
    set(GODOT_EXPORT_EXTENSION "zip")
  elseif(GODOT_EXPORT_PLATFORM STREQUAL "Windows")
    set(GODOT_EXPORT_EXTENSION "exe")
  endif()

  add_custom_target("export"
    COMMAND "${GODOT_EXE}"
    "--path"
    "${PROJECT_SOURCE_DIR}/godot"
    "--export"
    "${GODOT_EXPORT_PLATFORM}"
    "${PROJECT_SOURCE_DIR}/export/${GODOT_EXPORT_PLATFORM}/${PROJECT_NAME}.${GODOT_EXPORT_EXTENSION}"
    DEPENDS "scripts"
    SOURCES "${PROJECT_SOURCE_DIR}/godot/project.godot"
  )
endif()

### Run

if(GODOT_EXE)
  add_custom_target("run"
    COMMAND "${GODOT_EXE}"
    "--path"
    "${PROJECT_SOURCE_DIR}/godot"
    "${PROJECT_SOURCE_DIR}/godot/scenes/Game.tscn"
    DEPENDS "scripts"
    SOURCES "${PROJECT_SOURCE_DIR}/godot/project.godot"
  )
endif()