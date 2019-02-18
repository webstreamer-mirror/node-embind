@echo off
pushd

set _dirname=%~dp0
set _PYTHON2=c:/python27
set _DEBUG=
set _ERROR=
if "%NODE_ENV%"=="Debug"  set _DEBUG=--debug 
echo "@%_DEBUG%#%NODE_ENV%">type.txt
if defined PYTHON set _PYTHON2=%PYTHON%

set _configure=
set _build=
set _rebuild=
set _project=

set _action=%1
set _project=%2


if "%_action%"=="--configure" (
  set _configure=Yes
)

if "%_action%"=="--rebuild" (
  set _rebuil d=Yes
)

if "%_action%"=="--build" (
  set _build=Yes
)

if "%_configure%%_rebuild%%_build%" EQU "" (
  
  if "%_project%" EQU "" (
    set _project=%_action%
    set _configure=Yes
    set _rebuild=Yes
  ) else (
    echo Invalid command arguments, see help
    call :Usage
    set errorlevel=1
    goto :_EXIT
    
  )
)



REM echo _configure=%_configure%
REM echo _build=%_build%
REM echo _rebuild=%_rebuild%
REM echo _project=%_project%

set errorlevel=0
if "%_project%" EQU "" (
  call :BuildAll
) else (
  call :BuildOne %_dirname%%_project%
)


:_EXIT
popd
if defined _ERROR echo "%_ERROR%"
goto :eof

::--------------------------------------------------------
::-- Function section starts below here
::--------------------------------------------------------

:BuildOne    - build addon

set addon_dir=%1

if "%_configure%" == "Yes" (

  call npx node-gyp configure -C %addon_dir% --python %_PYTHON2% %_DEBUG%
  
  if %errorlevel% NEQ 0 (
    echo "configure failed (%addon_dir%)"
    goto :_EXIT
  )
)

if "%_build%" == "Yes" (

  call npx node-gyp build -C %addon_dir% --python %_PYTHON2% %_DEBUG%
  if %errorlevel% NEQ 0 (
    echo  "build failed (%addon_dir%)"
    goto :_EXIT
  )
)

if "%_rebuild%" == "Yes" (

  call npx node-gyp rebuild -C %addon_dir% --python %_PYTHON2% %_DEBUG%
  if %errorlevel% NEQ 0 (
    echo "rebuild failed (%addon_dir%)"
    goto :_EXIT
  )
)

goto:eof

:BuildAll - build all addon folders


for /f "delims=" %%c in ('dir /D /b "%_dirname%"') do (  
  if exist %_dirname%\%%c\binding.gyp (
     call :BuildOne %_dirname%\%%c
  ) 
)

goto :eof

:Usage 
  echo Usage: build [--build, --rebuild, --configure ] [project]
  echo only do configure for hello project : build --configure hello
  echo do build for all project in addons  : build --build
  echo.

goto :eof

