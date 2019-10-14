@echo off
rem Build 64
cls

rem Paths
rem set version=2017\Enterprise\
set version=2019\Community\
set cur_path=%cd%\
set env_path=%ProgramFiles(x86)%\Microsoft Visual Studio\%version%
set vars_path=%env_path%VC\Auxiliary\Build\

rem Setup
cd %vars_path%
call vcvars64.bat

rem Build
cd %env_path%
call msbuild.exe %*

rem Exit
cd %cur_path%
pause
exit /B %ERRORLEVEL%
