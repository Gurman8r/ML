@echo off
cd .\bin\Release\x64\
start ML_Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )