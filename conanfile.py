import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class MoonBuggy2DConan(ConanFile):
    name = "Moon Buggy 2D"
    version = "1.0.0"
    url = "https://github.com/codergal89/moon-buggy-2d"
    license = "MIT"
    description = "A 2D port of the classic game 'Moon Buggy'"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
    }
    generators = (
        "CMakeDeps"
    )
    settings = (
        "os",
        "arch",
        "compiler",
        "build_type",
    )
    tool_requires = (
        "cmake/[~3.25]",
        "ninja/[~1.11]",
    )

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        toolchain.variables["MOON_BUGGY_2D_VERSION"] = self.version
        toolchain.variables["MOON_BUGGY_2D_DESCRIPTION"] = self.description
        toolchain.variables["MOON_BUGGY_2D_BIN_DIR"] = os.path.join(self.recipe_folder, "bin")
        toolchain.generate()

    def layout(self):
        cmake_layout(self, generator="Unix Makefiles", src_folder="scripts")

    def package(self):
        cmake = CMake(self)
        cmake.install()
