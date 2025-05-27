from conan import ConanFile
from conan.tools.cmake import cmake_layout


class DNSUpdateTool(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("curlpp/0.8.1.cci.20240530")
        self.requires("nlohmann_json/3.12.0")

    def layout(self):
        cmake_layout(self)
