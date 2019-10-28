@echo off
cls
set CWD=%cd%\
pushd %CWD%

rem 2015 / 2017 / 2019
set vs_version=%1

rem Enterprise / Community
set vs_release=%2

rem Debug / Release
set config=%3

rem x86 / x64
set platform=%4

rem v140 / v141 / v142
set toolset=%5

rem Console / Windows
set subsystem=%6

echo Version 	Visual Studio %vs_version% %vs_release%
echo Configure 	%config%
echo Platform 	%platform%
echo Toolset 	%toolset%
echo Subsystem 	%subsystem%

set msbuild_path=%ProgramFiles(x86)%\Microsoft Visual Studio\%vs_version%\%vs_release%\

cd %msbuild_path%VC\Auxiliary\Build\
if "%platform%" == "x86" 	( call "vcvarsall.bat" x86 )
if "%platform%" == "x64" 	( call "vcvarsall.bat" x64 )
if "%platform%" == "Win32" 	( call "vcvarsall.bat" x86 )
if "%platform%" == "Win64" 	( call "vcvarsall.bat" x64 )

cd %msbuild_path%
call msbuild.exe "%CWD%..\ML_vs%vs_version%.sln" ^
 "/p:Configuration=%config%" ^
 "/p:PlatformTarget=%platform%" ^
 "/p:PlatformToolset=%toolset%" ^
 "/p:Subsystem=%subsystem%"

popd
if %ERRORLEVEL% NEQ 0 ( pause )