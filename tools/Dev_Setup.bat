rem INSTALL.bat
@echo off
cls

: initialize
set WorkingDir=%cd%\
set OpenALPath=C:\Windows\System32\OpenAL32.dll
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\


:check_MSBuild
if exist "%MSBuildPath%" (
	echo "Found %MSBuildPath%"
) else (
	echo "Couldn't find %MSBuildPath%"
	pause
	exit 1
)

: check_VC_Targets
if exist "%VCTargetsPath%" (
	echo "Found %VCTargetsPath%"
) else (
	echo "Couldn't find %VCTargetsPath%"
	pause
	exit 1
)

: check_VC_Vars
if exist "%VCVarsPath%" (
	if exist "%VCVarsPath%vcvars32.bat" (
		echo "Found %VCVarsPath%vcvars32.bat"
	) else (
		echo "Couldn't find %VCVarsPath%vcvars32.bat"
		pause
		exit 1
	)
	if exist "%VCVarsPath%vcvars64.bat" (
		echo "Found %VCVarsPath%vcvars64.bat"
	) else (
		echo "Couldn't find %VCVarsPath%vcvars64.bat"
		pause
		exit 1
	)
) else (
	echo "Couldn't find %VCVarsPath%"
	pause
	exit 1
)


:install_OpenAL
if exist "%OpenALPath%" (
	echo "Found %OpenALPath%"
) else (
	if exist "..\thirdparty\tools\oalinst.exe" (
		cd ..\thirdparty\tools\
		call oalinst.exe
		cd %WorkingDir%
	) else (
		echo "Couldn't find oalinst.exe"
		pause
		exit 1
	)
)

echo OK
pause
exit 0