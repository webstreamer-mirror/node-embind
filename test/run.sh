#!/bin/bash
__dir__=$( cd $( dirname ${BASH_SOURCE[0]} ) && pwd )

project=string
platform=linux
build=./test/addons/build.sh
_uname=$(uname)

[[ $_uname == MSYS_NT* ]] && platform=windows
[[ $platform == 'windows' ]] && build='./test/addons/build.cmd'

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
echo "===================================="


cd $__dir__/..

if [ $project == '*' ]; then
  [[ $_build == Yes ]] && $build
else
  [[ $_build == Yes ]] && $build $project
fi
ret=$?
[ $_build == Yes ] && [ $ret -ne 0 ] && echo "Build failed !" && exit 1


[[ $_test == Yes ]] && npx mocha ./test/$project.test.js
