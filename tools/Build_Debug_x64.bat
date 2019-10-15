@echo off
rem Build Debug x64
cls
set CWD=%cd%\
pushd %CWD%

rem set version=2017\Enterprise
set version=2019\Community

set env_path=%ProgramFiles(x86)%\Microsoft Visual Studio\%version%\
set aux_path=%env_path%VC\Auxiliary\Build\

cd %aux_path%
call vcvars64.bat

cd %env_path%
call msbuild.exe "%CWD%..\ML.sln" ^
 "/p:Configuration=Debug" ^
 "/p:PlatformTarget=x64" ^
 "/p:PlatformToolset=v142" ^
 "/p:Subsystem=Windows"

popd
if %ERRORLEVEL% NEQ 0 ( pause )