@echo off
rem Build_32.bat
cls

rem Visual Studio .NET 2002 (Toolset = 'v70' )
rem Visual Studio .NET 2003 (Toolset = 'v71' )
rem Visual Studio 2005      (Toolset = 'v80' )
rem Visual Studio 2008      (Toolset = 'v90' )
rem Visual Studio 2010      (Toolset = 'v100')
rem Visual Studio 2012      (Toolset = 'v110')
rem Visual Studio 2013      (Toolset = 'v120')
rem Visual Studio 2015      (Toolset = 'v140')
rem Visual Studio 2017      (Toolset = 'v141')

rem Paths
set cur_path=%cd%\
set env_path=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set vars_path=%env_path%VC\Auxiliary\Build\

rem Setup
cd %vars_path%
call vcvars32.bat

rem Build
cd %env_path%
call msbuild.exe %*

rem Exit
cd %cur_path%
pause
exit /B %ERRORLEVEL%