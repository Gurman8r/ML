@echo off
rem Build Debug x86
cls
call Build.bat ..\ML.sln "Configuration=Debug" "Target=x86" "Toolset=v141"
exit %ERRORLEVEL%