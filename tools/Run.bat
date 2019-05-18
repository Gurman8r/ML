@echo off
rem Run.bat
cls

rem Set Target Name
set TargetName=%1

rem Set Configuration
set Configuration=%2

rem Set Platform Target
set PlatformTarget=%3


rem Validate Platform Target
if "%TargetName%"=="" (
	echo Target Name cannot be empty!
	pause
	exit 1
) else (
	echo Target Name: "%TargetName%"
)

rem Validate Configuration
if "%Configuration%"=="" (
	echo Configuration cannot be empty!
	pause
	exit 1
) else (
	echo Configuration: "%Configuration%"
)

rem Validate Platform Target
if "%PlatformTarget%"=="" (
	echo Platform Target cannot be empty!
	pause
	exit 1
) else (
	echo Platform Target: "%PlatformTarget%"
)


rem Set File Path (.\bin\Debug\x86)
set FilePath=.\bin\%Configuration%\%PlatformTarget%

rem Set File Name (Example_Debug_x86.exe)
set FileName=%TargetName%_%Configuration%_%PlatformTarget%.exe

rem Run Executable
if exist %FilePath%\%FileName% (
	cd %FilePath%
	start %FileName%
	exit %ERRORLEVEL%
) else (
	echo File Not Found: "%FilePath%\%FileName%"
	pause
	exit 1
)