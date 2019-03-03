#!/bin/sh

_dirname=$( cd $( dirname ${BASH_SOURCE[0]} ) && pwd )
sudo -E docker run  --name node_embind_runner --env NODE_ENV=$NODE_ENV \
     --rm -v $_dirname:/home/ci/project webstreamer/node-gyp-gcc54 \
     bash -c " source /opt/nvm/nvm.sh && cd project && $1"
