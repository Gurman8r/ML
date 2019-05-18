@echo off
rem Build_Release_x86.bat
cls
call Build.bat ..\ML.sln Release x86
exit %ERRORLEVEL%