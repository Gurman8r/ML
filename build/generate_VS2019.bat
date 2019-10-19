@echo off
cd ..\
call .\build\premake5\Windows\premake5.exe vs2019
if %ERRORLEVEL% NEQ 0 ( pause )