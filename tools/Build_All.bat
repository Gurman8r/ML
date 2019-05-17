rem Build_All.bat
@echo off
cls

start Build_Debug_x64.bat
start Build_Debug_x86.bat
start Build_Release_x64.bat
start Build_Release_x86.bat

exit %ERRORLEVEL%