@echo off
call .\thirdparty\premake\premake5.exe vs2019
if %ERRORLEVEL% NEQ 0 (pause)