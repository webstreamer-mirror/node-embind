

_DOCKER_IMAGE='webstreamer/node-gyp-gcc54:0.1'
_PROJECT_DIR=$PWD
_DISABLE_PULL=
_COMMANDS=
#sudo -E docker run --rm -v $PWD:/home/conan/project webstreamer/node-gyp-gcc54:0.1  
#/bin/bash -c "source /home/conan/.nvm/nvm.sh && cd project && pyenv local 2.7.15 
#&&  npm install && node ./test/addons/build.js hello && npm test"
_HELP='
Usage:  docker-run [OPTIONS] command

A self-sufficient runtime for containers

Options:
      --config string      Location of client config files (default "/home/ci/snap/docker/321/.docker")
  -i, --docker-image       docker image nmes
  -p, --project-dir        project dir which will be mirror to docker and as work directory
  -D, --disable-pull-image disable pull docker image, use the pulled one
  command                  this string will run in bash -c "<command-str>"
'
while true ; do
    case "$1" in
      -i|--docker-image) 
        _DOCKER_IMAGE=$2; shift 2
      ;;

      -p|--project-dir)
        _PROJECT_DIR=$2; shift 2
      ;;

      -D |--disable-pull-image)
        _DISABLE_PULL='Yes'; shift
      ;;

      -h | --help)
         echo $_HELP
         exit 1
      ;;

      -*)
        echo "invlid options : $1"
        echo "use docker-run --help"
        echo $_HELP
        exit 1
      ;;

      *)
        _COMMANDS=$@
        break 
      ;;
    esac
done


echo _DOCKER_IMAGE $_DOCKER_IMAGE
echo _PROJECT_DIR  $_PROJECT_DIR
echo _DISABLE_PULL $_DISABLE_PULL
echo "\"$@\""
if [ -z $_DISABLE_PULL ]; then
   echo sudo -E docker pull $_DOCKER_IMAGE
fi

sudo -E docker run --rm -v $_PROJECT_DIR:/home/conan/project $_DOCKER_IMAGE \
  /bin/bash -c "source /home/conan/.nvm/nvm.sh && cd project \
  && echo '        ================================='\
  && echo \"        $_DOCKER_IMAGE\"\
  && echo '        ================================='\
  && $@"

 
