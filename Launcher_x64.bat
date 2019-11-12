@echo off
cd .\bin\Release\x64\
start Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )