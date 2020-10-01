from conans import ConanFile, CMake


class MoonBuggy2DConan(ConanFile):
    settings = ("os", "compiler", "build_type", "arch")
    requires = ("godot-cpp/cci.3.2-20200130")
    generators = ("cmake")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
