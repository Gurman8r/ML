@echo off
rem Build.bat
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
set WorkingDir=%cd%\
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\


rem Solution
set Solution=%1
if not exist "%Solution%" (
	echo Solution Not Found "%Solution%"
	pause
	exit 1
)


rem Defaults
set Configuration=Debug
set Target=x86
set Toolset=v141
set ExitPause=1

set %2
set %3
set %4
set %5


rem Environment Variables
cd %VCVarsPath%
if "%Target%"=="x86" (
	call vcvars32.bat
) else (
	if "%Target%"=="x64" (
		call vcvars64.bat
	) else (
		echo Unknown Platform Target: %Target%
		pause
		exit 1
	)
)


rem Build Solution
cd %MSBuildPath%

call msbuild.exe %WorkingDir%%Solution% ^
	/p:Configuration=%Configuration% ^
	/p:PlatformTarget=%Target% ^
	/p:PlatformToolset=%Toolset%

cd %WorkingDir%

if "%ExitPause%"=="1" (
	pause
)

exit %ERRORLEVEL%