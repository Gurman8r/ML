@echo off
rem Run_Debug_x64.bat
cls

set TargetName=ML_Launcher
set Configuration=Debug
set PlatformTarget=x64

set ExePath=..\bin\%Configuration%\%PlatformTarget%\
set ExeName=%TargetName%_%Configuration%_%PlatformTarget%.exe

if not exist %ExePath%%ExeName% (
	echo File Not Found: "%ExePath%%ExeName%"
	pause
	exit 1
) else (
	echo %ExePath%%ExeName%
	cd %ExePath%
	call %ExeName% %*
	exit %ERRORLEVEL%
)