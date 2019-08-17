@echo off
rem Post_Build.bat

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set SolutionDir=%1
set TargetName=%2
set Configuration=%3
set PlatformTarget=%4

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set CurDir=%cd%/
set ExtDir=%SolutionDir%thirdparty
set LibDir=%SolutionDir%lib
set BinDir=%SolutionDir%bin
set OutDir=%SolutionDir%bin\%Configuration%\%PlatformTarget%

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

rem OpenAL
xcopy /y %ExtDir%\bin\OpenAL32.dll %OutDir%

rem Assimp
xcopy /y %ExtDir%\bin\assimp_%Configuration%_%PlatformTarget%.dll %OutDir%

rem PDcurses
xcopy /y %ExtDir%\bin\%Configuration%\pdcurses.dll %OutDir%

rem Python
if "%Configuration%"=="Debug" (
	xcopy /y %ExtDir%\bin\%Configuration%\%PlatformTarget%\python39_d.dll %OutDir%
) else (
	xcopy /y %ExtDir%\bin\%Configuration%\%PlatformTarget%\python39.dll %OutDir%
)

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *