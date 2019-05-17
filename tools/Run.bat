rem Run.bat
@echo off
cls

rem Run.bat Example Debug x86
set TargetName=%1
set Configuration=%2
set PlatformTarget=%3

rem .\bin\Debug\x86
set FilePath=.\bin\%Configuration%\%PlatformTarget%

rem Example_Debug_x86.exe
set FileName=%TargetName%_%Configuration%_%PlatformTarget%.exe

rem .\bin\Debug\x86\Example_Debug_x86.exe
set Executable=%FilePath%\%FileName%

if exist %Executable% (
	cd %FilePath%
	start %FileName%
	exit %ERRORLEVEL%
) else (
	echo Not Found: "%FilePath%\%FileName%"
	pause
	exit 1
)