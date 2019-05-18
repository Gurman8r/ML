@echo off
rem Build_Debug_x64.bat
cls
call Build.bat ..\ML.sln Debug x64
exit %ERRORLEVEL%