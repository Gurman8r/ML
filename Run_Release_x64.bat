rem Run_Release_x64.bat
@echo off

set TargetName=Launcher
set Configuration=Release
set PlatformTarget=x64

set FilePath=.\bin\%Configuration%\%PlatformTarget%
set FileName=%TargetName%_%Configuration%_%PlatformTarget%.exe

if not exist %FilePath%\%FileName% (
	echo File Not Found: "%FilePath%\%FileName%"
	pause
	exit 1
)

cd %FilePath%
start %FileName%
exit %ERRORLEVEL%