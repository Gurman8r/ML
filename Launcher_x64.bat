@echo off
cls

set Configuration=Release

set PlatformTarget=x64

set ExePath=.\bin\%Configuration%\%PlatformTarget%\

set ExeName=ML_Launcher_%Configuration%_%PlatformTarget%.exe

if not exist %ExePath%%ExeName% (
	echo File Not Found: "%ExePath%%ExeName%"
	pause
	exit 1
) else (
	cd %ExePath%
	start %ExeName% %*
)