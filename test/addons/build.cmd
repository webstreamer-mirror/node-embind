@echo off
pushd

set _dirname=%~dp0
set _PYTHON2=c:/python27
set _DEBUG=
set _ERROR=

if "%NODE_ENV%" EQU Debug  set _DEBUG=--debug 

if defined PYTHON set _PYTHON2=%PYTHON%

if "%1" EQU "" (
  call :BuildAll 
) else (
  call :BuildOne %_dirname%%1
)


:_EXIT
popd
goto :eof

::--------------------------------------------------------
::-- Function section starts below here
::--------------------------------------------------------

:BuildOne    - build addon
set addon_dir=%1

call npx node-gyp configure -C %addon_dir% --python %_PYTHON2%
if %errorlevel% NEQ 0 (
  set _ERROR="configure failed (%addon_dir%) !"
  goto :_EXIT
)

call npx node-gyp rebuild -C %addon_dir% --python %_PYTHON2%
if %errorlevel% NEQ 0 (
  set _ERROR="build failed (%addon_dir%) !"
  goto :_EXIT
)
goto:eof

:BuildAll - build all addon folders

for  /D  %%s in (%_dirname%\*)do (
   set addon_dir=%%s
   if exist %addon_dir%\binding.gyp (
      call :BuildOne %addon_dir%
   ) else (
     echo skip %addon_dir% as no binding.gyp
   )
)
goto :eof

