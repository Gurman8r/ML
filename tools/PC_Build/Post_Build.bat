@echo off
rem Post_Build.bat

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set SolutionDir=%1
set TargetName=%2
set Configuration=%3
set PlatformTarget=%4

set Cur=%cd%/
set Ext=%SolutionDir%thirdparty
set Lib=%SolutionDir%lib
set Bin=%SolutionDir%bin
set Out=%SolutionDir%bin\%Configuration%\%PlatformTarget%

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

rem OpenAL
call :copy_if %Ext%\bin\OpenAL32.dll %Out%

rem Assimp
call :copy_if %Ext%\bin\assimp_%Configuration%_%PlatformTarget%.dll %Out%

rem PDcurses
call :copy_if %Ext%\bin\%Configuration%\pdcurses.dll %Out%

rem Python
if "%Configuration%"=="Debug" (
 call :copy_if %Ext%\bin\%Configuration%\%PlatformTarget%\python39_d.dll %Out%
 call :copy_if %Ext%\bin\%Configuration%\%PlatformTarget%\python_d.exe %Out%
) else (
 call :copy_if %Ext%\bin\%Configuration%\%PlatformTarget%\python39.dll %Out%
 call :copy_if %Ext%\bin\%Configuration%\%PlatformTarget%\python.exe %Out%
)

exit /B %ERRORLEVEL%

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

:copy_if
if not exist %~1 ( xcopy /y %~1 %~2 )

rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *