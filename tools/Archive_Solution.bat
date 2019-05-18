@echo off
rem Archive_Solution.bat
cls

rem Set Name
set ZipName=ML_Solution
if not "%1"=="" ( set ZipName=%1 )

rem Zip Git
set ZipGit=0
if not "%2"=="" ( set ZipGit=%2 )

rem Zip Tools
set ZipTools=0
if not "%3"=="" ( set ZipTools=%3 )

rem Set Paths
set WorkingDir=%cd%/
set OutputDir=%WorkingDir%
set OutputFile=%OutputDir%\%ZipName%.zip

rem Generate Archive
cd C:\Program Files\7-Zip

if "%ZipGit%"=="1" (
7z a -r %OutputFile% %WorkingDir%../.git
7z a 	%OutputFile% %WorkingDir%../README.md
)

if "%ZipTools%"=="1" (
7z a -r %OutputFile% %WorkingDir%../tools
)

7z a 	%OutputFile% %WorkingDir%../ML.sln
7z a 	%OutputFile% %WorkingDir%../ML_Config.ini
7z a 	%OutputFile% %WorkingDir%../*.bat
7z a -r %OutputFile% %WorkingDir%../assets
7z a -r %OutputFile% %WorkingDir%../include
7z a -r %OutputFile% %WorkingDir%../plugins
7z a -r %OutputFile% %WorkingDir%../proj
7z a -r %OutputFile% %WorkingDir%../src
7z a -r %OutputFile% %WorkingDir%../thirdparty

rem Exit
cd %WorkingDir%
exit %ERRORLEVEL%