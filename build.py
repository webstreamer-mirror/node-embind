#!/usr/bin/env python
#from conanos.build import Main

from cpt.packager import ConanMultiPackager,load_cf_class
import platform
import os
import re




print('==========================>>>')

print(os.system('pip list'))
print('==========================>>>')


os.environ['CONAN_ARCHS']='x86_64'
os.environ['CONAN_USERNAME'] = 'conan'
if platform.system() == 'Windows':
    os.environ['CONAN_VISUAL_VERSIONS'] = '15'
    os.environ['CONAN_VISUAL_RUNTIMES'] = 'MD,MDd'
else:
    os.environ['CONAN_GCC_VERSIONS'] = '5'
    os.environ['CONAN_DOCKER_IMAGE'] = 'webstreamer/nvm-py2-gcc54:0.1'
    os.environ['CONAN_USE_DOCKER'] = '1'


def _filter(pkg_name , builder):

    items = []

    for settings, options, env_vars, build_requires, reference in builder.items:

        if settings['compiler.libcxx'] == 'libstdc++11' :
            items.append([settings, options, env_vars, build_requires])


    builder.items = items
    return builder

def Main(name):
    NODE_VERSION = os.environ.get('NODE_VERSION','10.15.1')
    
    docker_entry_script = 'source /home/conan/.nvm/nvm.sh'
    ' && nvm install {0}'
    ' && nvm use {0}'.format(NODE_VERSION)

    builder = ConanMultiPackager(docker_entry_script=docker_entry_script)
    builder.add_common_builds(pure_c=False)
    builder = _filter(name, builder)

    builder.run()

if __name__ == "__main__":    
    Main('node-embind')