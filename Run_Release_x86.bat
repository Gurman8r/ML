@echo off
rem Run_Release_x86.bat
cls

set TargetName=Launcher
set Configuration=Release
set PlatformTarget=x86

set ExePath=.\bin\%Configuration%\%PlatformTarget%
set ExeName=%TargetExeName%_%Configuration%_%PlatformTarget%.exe
set ExeFull=%ExePath%\%ExeName%

if not exist %ExeFull% (
	echo File Not Found: "%ExeFull%"
	pause
	exit 1
) else (
	echo Running "%ExeFull%"...
	cd %ExePath%
	start %ExeName%
	exit %ERRORLEVEL%
)