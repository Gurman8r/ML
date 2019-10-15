@echo off
rem Clean All
cls

set CurDir=%cd%\
set BinDir=%CurDir%\..\bin
set LibDir=%CurDir%\..\lib
set IntDir=%CurDir%\..\obj

if exist "%BinDir%" (
	rd /s /q %BinDir%
	echo Cleaned "%BinDir%"
)

if exist "%LibDir%" (
	rd /s /q %LibDir%
	echo Cleaned "%LibDir%"
)

if exist "%IntDir%" (
	rd /s /q %IntDir%
	echo Cleaned "%IntDir%"
)

cd %CurDir%
exit /B %ERRORLEVEL%