@echo off
cd .\bin\Release\x86\
start ML_Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )