import os
import shutil
import platform
from conans import CMake, tools
from conans import ConanFile


class NodeEmbind(ConanFile):
    name = "node-embind"
    version = "0.1.0"
    branch = "master"
    # generators = "cmake"
    settings = "os", "compiler", "arch", "build_type"
    # options = {}
    # default_options = "shared=True"
    exports = ["LICENSE"]
    exports_sources = ["config/*","include/*","src/*","test/*","package.json"]
    url = "https://github.com/webstreamer/node-embind"
    license = "MIT"
    description = "node-embind is emscripten binding with embind interface for Node.JS"

    source_subfolder = "source_subfolder"

    def call(self, cmd ):

        CONAN_DOCKER_IMAGE = os.environ.get('CONAN_DOCKER_IMAGE','')
        if platform.system() == 'Linux':
            if CONAN_DOCKER_IMAGE.startswith('webstreamer/node-gyp-gcc54'):
                self.run('bash -c "source /home/conan/.nvm/nvm.sh && %s"'%cmd)
                return
        self.run(cmd)

    def config(self):
        if platform.system() == 'Linux': 
            self.settings.compiler.libcxx = 'libstdc++11'

    def source(self):
        self.run('echo ========================')
        self.run('pwd')
        self.run('pyenv local 2.7.15')
        self.run('echo ========================')

        return

        self.call('npm install')

    def build(self):
        return
        options = ''
        
        if self.settings.build_type == 'Debug':
            options +=' --debug'

        self.call('pyenv local 2.7.15 && python2 --version && node test/addons/build.js hello %s'%options)

        if self.settings.build_type == 'Debug':
            self.call('npm run test:debug')
        else:
            self.call('npm test')



    def package(self):
        pass

    def package_info(self):
        pass

