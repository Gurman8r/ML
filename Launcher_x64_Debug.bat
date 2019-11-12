@echo off
cd .\bin\Debug\x64\
call Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )