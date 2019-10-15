@echo off
rem Run_Debug_x64.bat
cls

set Configuration=Debug
set PlatformTarget=x64
set ExePath=..\bin\%Configuration%\%PlatformTarget%\
set ExeName=ML_Launcher_%Configuration%_%PlatformTarget%.exe

if not exist %ExePath%%ExeName% (
	echo File Not Found: "%ExePath%%ExeName%"
	pause
	exit 1
) else (
	echo %ExePath%%ExeName%
	cd %ExePath%
	start %ExeName% %*
	exit %ERRORLEVEL%
)