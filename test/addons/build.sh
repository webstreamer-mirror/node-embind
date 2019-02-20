#!/bin/sh

_dirname=$( cd $( dirname ${BASH_SOURCE[0]} ) && pwd )

_PYTHON2=
_DEBUG=
_ERROR=
_CONFIG=Release

[ "$NODE_ENV" == Debug ] && _DEBUG=--debug && _CONFIG=Debug
[ ! -z "$PYTHON" ] && _PYTHON2="--python $PYTHON"


_configure=
_build=
_rebuild=
_project=
_errorno=0

function _doConfigure(){
 local folder=$1 
 local name=$(basename $folder)

 [ -z $_configure ] && return 0;

 echo "[$name] configure"

 [ -f $folder/build/Makefile ] && rm -f $folder/build/Makefile

 npx node-gyp configure -C $folder $_DEBUG $_PYTHON2

 if [ ! -f $folder/build/Makefile ]; then
   echo "[$name] configure failed."
   _errorno=10;
 fi
 return $_errorno
}

function _doBuild(){

  local folder=$1
  local name=$(basename $folder)
  local subcommand=


  [ -n $_build ] && subcommand=build
  [ -n $_rebuild ] && subcommand=rebuild

  [ -z $subcommand ] && return 0

  echo "[$name] $subcommand"

  local addon=$folder/build/$_CONFIG/{$name}.node

  [ -f  $addon ] && rm -rf $addon

  npx node-gyp rebuild -C $folder $_DEBUG

  [ -f $addon ] && return 0

  echo "[$name] $subcommand failed."
  _errorno=11;

  return $_errorno
}

function BuildOne(){

  _doConfigure $1 &&  _doBuild $1

}

function BuildAll(){

  for i in $(find $_dirname -mindepth 1 -maxdepth 1  -type d)
  do

    if [ -f $i/binding.gyp ]; then
        BuildOne $i
        [ $_errorno -ne 0 ] && return $_errorno
    fi
  done
}

#-----------------------------------
#   Main
#-----------------------------------

_HELP='
Usage: build [--build, --rebuild, --configure ] [project]
       only do configure for hello project : build --configure hello
       do build for all project in addons  : build --build
'
while true ; do
    case "$1" in
      --configure) 
        _configure=Yes; shift 1;
      ;;

      --build) 
        _build=Yes; shift 1;
      ;;
      
      --rebuild) 
        _rebuild=Yes; shift 1;
      ;;
      
      --help) 
        echo $_HELP;
        exit/b 0
      ;;


      -*)
        echo "invlid options : $1"
        echo "use docker-run --help"
        echo $_HELP
        exit/b 1
      ;;

      *)
        _project=$1
        break 
      ;;
    esac
done

if [ "$_rebuild$_build$_configure" == "" ]; then
  _configure=Yes
  _rebuild=Yes
fi

if [ "$_project" == "" ]; then
  BuildAll 
else

  BuildOne $_dirname/$_project
fi

exit $_errorno