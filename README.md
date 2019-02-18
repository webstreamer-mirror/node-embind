# node-embind


# Build Enviroment

## Linux

* GCC >=5.4
* Python >= 2.7 (python3 not support)
* NodeJs >= 10

Ubuntu install example (verified in 18.04)

_China user may need to change the source to [aliyun](!https://opsx.alibaba.com/mirror)_

```sh
sudo apt-get install  build-essential # for GCC
sudo apt-get install  git
sudo apt-get install  python
sudo apt-get install  python-pip
wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.34.0/install.sh | bash

# reset your bash
nvm install 10
nvm use 10

```