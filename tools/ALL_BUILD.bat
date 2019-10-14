@echo off
rem Build All
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
rem Visual Studio 2019      (Toolset = 'v142')

start Build_Debug_x64.bat
start Build_Debug_x86.bat
start Build_Release_x64.bat
start Build_Release_x86.bat

exit /B %ERRORLEVEL%
