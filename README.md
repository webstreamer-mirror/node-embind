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
nvm install v10.15.1
nvm use v10.15.1
npm install -g node-gyp


```

# How to build

```sh
  npm install
  npm run build
  npm test
```

_China user can accelerate by taobao mirror :_ 
```sh 
  npm install --registry=https://registry.npm.taobao.org
  
```

if you want to see more detail about build, you can see the build.log, or use shell comamnd build.

Windows
```sh
  test\addons\build.cmd
```
Linux
```sh
  bash ./test/addons/build.sh
```


You can also use docker to build Linux version

```sh
  npm docker:pull # pull docker images
  npm docker:build
  npm docker:test
```
if you want to see the build details in console, use npm docker:build.log

Use enviroment NODE_ENV to select Debug (NODE_ENV=Debug) or Release version.

In case you didn't install node in host, you can directly use shell.

```sh
  bash ./docker.sh 'npm install'
  bash ./docker.sh 'bash ./test/addons/build.sh'
  bash ./docker.sh 'npm run test'
```