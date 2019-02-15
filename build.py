#!/usr/bin/env python
#from conanos.build import Main

from cpt.packager import ConanMultiPackager,load_cf_class
import platform
import os
import re


def default_env():
    os.environ['CONAN_ARCHS']=os.environ.get('CONAN_ARCHS','x86_64')
    os.environ['CONAN_USERNAME']=os.environ.get('CONAN_USERNAME','webstreamer')
    if platform.system() == 'Windows':
        os.environ['CONAN_VISUAL_VERSIONS'] = '15'
        os.environ['CONAN_VISUAL_RUNTIMES'] = 'MD,MDd'
    else:
        os.environ['CONAN_GCC_VERSIONS'] = os.environ.get('CONAN_GCC_VERSIONS','5')
        os.environ['CONAN_DOCKER_IMAGE'] = os.environ.get('CONAN_DOCKER_IMAGE','webstreamer/node-gyp-gcc54:0.1')
        os.environ['CONAN_USE_DOCKER'] = '1'


def filter(builder):

    items = []

    for settings, options, env_vars, build_requires, reference in builder.items:
        if platform.system() == 'Linux':
            if not settings['compiler.libcxx'] == 'libstdc++11' :
                continue

        items.append([settings, options, env_vars, build_requires])


    builder.items = items
    return builder

def Main():
    default_env()
    docker_entry_script=''
    builder = ConanMultiPackager(docker_entry_script=docker_entry_script)
    builder.add_common_builds(pure_c=False)
    builder = filter(builder)
    builder.run()

if __name__ == "__main__":    
    Main()