@echo off
rem Clean.bat
cls

rem Set Paths
set WorkingDir=%cd%\

rem Clean Binary Directory
set BinDir=%WorkingDir%..\bin
if exist "%BinDir%" (
	rd /s /q %BinDir%
	echo Cleaned "%BinDir%"
)

rem Clean Library Directory
set LibDir=%WorkingDir%..\lib
if exist "%LibDir%" (
	rd /s /q %LibDir%
	echo Cleaned "%LibDir%"
)

rem Clean Intermediate Directory
set IntDir=%WorkingDir%..\obj
if exist "%IntDir%" (
	rd /s /q %IntDir%
	echo Cleaned "%IntDir%"
)

cd %WorkingDir%
exit %ERRORLEVEL%