import os
import shutil

from conans import CMake, tools
from conans import ConanFile


class NodeEmbind(ConanFile):
    name = "node-embind"
    version = "0.1.0"
    branch = "master"
    generators = "cmake"
    settings = "os", "compiler", "arch", "build_type"
    # options = {}
    # default_options = "shared=True"
    exports = ["LICENSE"]
    exports_sources = ["config/*","include/*","src/*","test/*","package.json"]
    url = "https://github.com/webstreamer/node-embind"
    license = "MIT"
    description = "node-embind is emscripten binding with embind interface for Node.JS"

    source_subfolder = "source_subfolder"

    def config(self):
        del self.settings.compiler.libcxx

    def source(self):
        self.run('npm install')

    def build(self):
        options = ''
        
        if self.settings.build_type == 'Debug':
            options +=' --debug'

        self.run('node test/addons/build.js hello %s'%options)

        if self.settings.build_type == 'Debug':
            self.run('npm run test:debug')
        else:
            self.run('npm test')



    def package(self):
        pass

    def package_info(self):
        pass

