@echo off
rem Build Release x86
cls
call Build.bat ..\ML.sln "Configuration=Release" "Target=x86" "Toolset=v141" "ExitPause=0"
exit %ERRORLEVEL%