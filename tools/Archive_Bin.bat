@echo off
rem Archive_Bin.bat
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

rem Set Paths
set WorkingDir=%cd%/
set OutputDir=%WorkingDir%
set OutputFile=%OutputDir%\%TargetName%_%Configuration%_%PlatformTarget%.zip

rem Generate Archive
cd %ProgramFiles%\7-Zip

7z a 	%OutputFile% %OutputDir%../Run_%Configuration%_%PlatformTarget%.bat
7z a 	%OutputFile% %OutputDir%../ML.ini
7z a 	%OutputFile% %OutputDir%../README.md
7z a 	%OutputFile% %OutputDir%../thirdparty/tools/oalinst.exe
7z a -r %OutputFile% %OutputDir%../assets/
7z a -r %OutputFile% %OutputDir%../*%Configuration%_%PlatformTarget%.exe*
7z a -r %OutputFile% %OutputDir%../*%Configuration%_%PlatformTarget%.dll*

rem Exit
cd %WorkingDir%
exit %ERRORLEVEL%