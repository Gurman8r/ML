@echo off & setlocal

set WorkingDir=%cd%\
set BinDir=%WorkingDir%..\bin
set LibDir=%WorkingDir%..\lib
set TmpDir=%WorkingDir%..\tmp

if exist "%BinDir%" (
	rd /s /q %BinDir%
	echo Cleaned "%BinDir%"
)

if exist "%LibDir%" (
	rd /s /q %LibDir%
	echo Cleaned "%LibDir%"
)

if exist "%TmpDir%" (
	rd /s /q %TmpDir%
	echo Cleaned "%TmpDir%"
)

exit %ERRORLEVEL%