@echo off

set ExeName=%1
set ExeConf=%2
set ExeCPU=%3

if "%ExeName%"=="" (
	exit 1
)
if "%ExeConf%"=="" (
	exit 1
)
if "%ExeCPU%"=="" (
	exit 1
)

set CurDir=%cd%/
set OutDir=%CurDir%

set ArchiveName=%ExeName%_%ExeConf%_%ExeCPU%
set ArchiveType=.zip

set OutFile=%OutDir%%ArchiveName%%ArchiveType%
set RunFile=Run_%ExeConf%_%ExeCPU%.bat

set ZipDir=%ProgramFiles%\7-Zip
cd %ZipDir%

rem 7z a 	%OutFile% %CurDir%%RunFile%
7z a 	%OutFile% %CurDir%../%RunFile%
7z a 	%OutFile% %CurDir%../ML_Config.ini
7z a 	%OutFile% %CurDir%../README.md
7z a -r %OutFile% %CurDir%../assets/
7z a -r %OutFile% %CurDir%../*%ExeConf%_%ExeCPU%.exe*
7z a -r %OutFile% %CurDir%../*%ExeConf%_%ExeCPU%.dll*

exit %ERRORLEVEL%