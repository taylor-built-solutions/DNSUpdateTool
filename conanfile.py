from conan import ConanFile
from conan.tools.cmake import cmake_layout


class DNSUpdateTool(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("cpr/1.11.2")
        self.requires("nlohmann_json/3.12.0")

    def layout(self):
        cmake_layout(self)
