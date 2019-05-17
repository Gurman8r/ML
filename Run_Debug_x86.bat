@echo off

set TargetName=Sandbox
set Configuration=Debug
set PlatformTarget=x86

set Path=.\bin\%Configuration%\%PlatformTarget%
set Name=%TargetName%_%Configuration%_%PlatformTarget%.exe
set File=%Path%\%Name%

if not exist %File% (
	echo File Not Found: "%File%"
	pause
	exit 1
) else (
	echo Opening... "%File%"
	cd %Path%
	start %Name%
	exit %ERRORLEVEL%
)