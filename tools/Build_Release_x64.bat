@echo off
rem Build Release x64
cls
call Build.bat ..\ML.sln "Configuration=Release" "Target=x64" "Toolset=v141"
exit %ERRORLEVEL%