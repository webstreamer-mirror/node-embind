#!/bin/sh

sudo -E docker run --env NODE_ENV=$NODE_ENV --rm -v $PWD:/home/ci/project webstreamer/node-gyp-gcc54 \
  bash -c " source /opt/nvm/nvm.sh && cd project && $1"
