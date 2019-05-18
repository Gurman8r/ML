@echo off
rem Build_Debug_x86.bat
cls
call Build.bat ..\ML.sln Debug x86
exit %ERRORLEVEL%