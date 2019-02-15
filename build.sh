sudo -E docker run --rm -v $PWD:/home/conan/project webstreamer/node-gyp-gcc54:0.1  /bin/bash -c "source /home/conan/.nvm/nvm.sh && cd project && pyenv local 2.7.15 &&  npm install && node ./test/addons/build.js hello && npm test"

