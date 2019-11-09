@echo off
cd .\bin\Debug\x86\
call ML_Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )