@echo off
rem Build_Release_x64.bat
cls
call Build.bat ..\ML.sln Release x64
exit %ERRORLEVEL%