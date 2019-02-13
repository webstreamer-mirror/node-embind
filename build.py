#!/usr/bin/env python
from conanos.build import Main
import platform
import os

os.environ['CONAN_ARCHS']='x86_64'
if platform.system() == 'Windows':
    os.environ['CONAN_VISUAL_VERSIONS'] = '15'
    os.environ['CONAN_VISUAL_RUNTIMES'] = 'MD,MDd'

if __name__ == "__main__":    
    Main('node-embind')