#!/usr/bin/env python
#from conanos.build import Main

from cpt.packager import ConanMultiPackager,load_cf_class
import platform
import os
import re


os.environ['CONAN_ARCHS']='x86_64'
os.environ['CONAN_USERNAME'] = 'webstreamer'
if platform.system() == 'Windows':
    os.environ['CONAN_VISUAL_VERSIONS'] = '15'
    os.environ['CONAN_VISUAL_RUNTIMES'] = 'MD,MDd'
else:
    os.environ['CONAN_GCC_VERSIONS'] = '5'
    os.environ['CONAN_DOCKER_IMAGE'] = 'webstreamer/nvm-py2-gcc54:0.1'
    os.environ['CONAN_USE_DOCKER'] = '1'
    
def Main(name):
    
    docker_entry_script = ''
    builder = ConanMultiPackager(docker_entry_script=docker_entry_script)
    builder.add_common_builds(pure_c=False)

    builder.run()

if __name__ == "__main__":    
    Main('node-embind')