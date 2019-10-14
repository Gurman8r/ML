@echo off
rem Build 64
cls
pushd %cd%\

rem set version=2017\Enterprise
set version=2019\Community

set env_path=%ProgramFiles(x86)%\Microsoft Visual Studio\%version%\

cd %env_path%VC\Auxiliary\Build\
call vcvars64.bat

cd %env_path%
call msbuild.exe %*

popd
pause