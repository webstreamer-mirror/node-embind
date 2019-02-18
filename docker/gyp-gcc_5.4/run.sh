#!/usr/bin/env bash
sudo docker run --rm -v $PWD:/home/ci/project -it webstreamer/gyp-gcc54 /bin/bash
