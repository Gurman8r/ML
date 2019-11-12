@echo off
cd .\bin\Release\x86\
start Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )