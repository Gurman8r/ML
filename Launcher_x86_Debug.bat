@echo off
cd .\bin\Debug\x86\
call Launcher.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )