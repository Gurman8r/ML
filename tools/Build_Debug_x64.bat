@echo off
rem Build Debug x64
cls
call Build.bat ..\ML.sln "Configuration=Debug" "Target=x64" "Toolset=v141"
exit %ERRORLEVEL%