@echo off
rem Run_Release_x86.bat
cls

set TargetName=Launcher
set Configuration=Release
set PlatformTarget=x86

set ExePath=.\bin\%Configuration%\%PlatformTarget%\
set ExeName=%TargetName%_%Configuration%_%PlatformTarget%.exe

if not exist %ExePath%%ExeName% (
	echo File Not Found: "%ExePath%%ExeName%"
	pause
	exit 1
) else (
	echo %ExePath%%ExeName%
	cd %ExePath%
	call %ExeName%
	exit %ERRORLEVEL%
)