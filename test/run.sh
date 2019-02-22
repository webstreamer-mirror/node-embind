#!/bin/bash
__dir__=$( cd $( dirname ${BASH_SOURCE[0]} ) && pwd )

project=string
platform=linux
build=./test/addons/build.sh
_uname=$(uname)

[[ $_uname == MSYS_NT* ]] && platform=windows
[[ $_uname == MINGW*_NT* ]] && platform=windows


_build=Yes
_test=Yes

#-----------------------------------
#   Main
#-----------------------------------

_HELP='
'
while true ; do
    case "$1" in
      t | -t|--test) 
        _test=Yes; shift 1;
		_build=No
      ;;

      b | -b|--build) 
        _build=Yes; shift 1;
		_test=No
      ;;
	  *)
       project=$1
       break 
      ;;
    esac
done

[[ -z $project ]] && project=string
echo "===================================="
echo "    project : $project              "
echo "    Build   : $_build               "
echo "    Test    : $_test                "
echo "    NODE_ENV: $NODE_ENV             "
echo "    Platform: $platform             "
echo "===================================="


cd $__dir__/..

_project=$project
[[ $project == 'all' ]] && _project=


if [[ $_build == Yes ]];then
  
  if [[ $platform == 'windows' ]]; then
	cmd.exe /C ".\\test\\addons\\build.cmd $_project"
  else
	bash ./test/addons/build.sh $_project
  fi
fi

ret=$?
[ $_build == Yes ] && [ $ret -ne 0 ] && echo "Build failed !" && exit 1

[[ $project == 'all' ]] && _project='*'
[[ $_test == Yes ]] && npx mocha ./test/$_project.test.js
