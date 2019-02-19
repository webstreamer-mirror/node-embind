@echo off
pushd

set _dirname=%~dp0
set _PYTHON2=c:/Python27
set _DEBUG=
set _ERROR=
set _ERRORNO=0
set _BuildFolder=Release
if "%NODE_ENV%"=="Debug"  (
  set _DEBUG=--debug 
  set _BuildFolder=Debug
)  

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


::========================================
::  Main
::========================================
if "%_project%" EQU "" (
  call :_buildAll
) else (
  call :_buildOne %_dirname%%_project%
)


::---------------------------------------
::            End of Script
::---------------------------------------
:_EXIT
popd

exit/b %_ERRORNO%

goto :eof

::---------------------------------------
::            BuildOne
::---------------------------------------

:_buildOne    - build addon

call :_doConfigure %1 %2

call :_doBuild %1 %2

goto :eof


::---------------------------------------
::            buildAll
::---------------------------------------
:_buildAll - build all addon folders

for /f "delims=" %%c in ('dir /D /b "%_dirname%"') do (  
  if exist %_dirname%\%%c\binding.gyp (
     call :_buildOne %_dirname%\%%c %%c
  ) 
)

goto :eof

:Usage 
  echo Usage: build [--build, --rebuild, --configure ] [project]
  echo only do configure for hello project : build --configure hello
  echo do build for all project in addons  : build --build
  echo.

goto :eof




::  ---------------------------------------
::              Configure
::  ---------------------------------------
:_doConfigure

  set folder=%1
  set name=%2
  
  if "%_configure%"=="" goto :eof
   
  echo [%name%] configure
    
  if exist %folder%\build\*.sln del /Q %folder%\build\*.sln
  
  call npx node-gyp configure -C %folder% --python %_PYTHON2% %_DEBUG%
  
  if exist %folder%\build\*.sln goto :eof
 
  ::error
  echo "configure failed (%folder%)"
  set _ERRORNO=10
  goto :_EXIT

goto :eof


::---------------------------------------
::            Build
::---------------------------------------
:_doBuild
  set folder=%1
  set name=%2
  set subcommand=

  if "%_build%"   == "Yes" set subcommand=build
  if "%_rebuild%" == "Yes" set subcommand=rebuild
 
  if "%subcommand%"=="" goto :eof
 
  echo [%name%] %subcommand%

  set outd=%folder%\build\%_BuildFolder%
 
  if exist %outd%\*.node del/Q %outd%\*.node
 
  call npx node-gyp %subcommand% -C %folder% --python %_PYTHON2% %_DEBUG%
 
  if exist %outd%\*.node goto :eof
  echo  %outd%\*.node
 
  :: error
 
  echo "%subcommand% failed (%addon_dir%)"
  set _ERRORNO=11
  goto :_EXIT

goto :eof