@echo off

set WorkingDir=%cd%\

rem Install OpenAL
: installOpenAL
if not exist "C:\Windows\System32\OpenAL32.dll" (
	if not exist "..\thirdparty\tools\oalinst.exe" (
		echo OpenAL installer not found.
		pause
		exit 1
	) else (
		echo Installing OpenAL
		call ..\thirdparty\tools\oalinst.exe
	)
)

echo Done.
pause
exit %ERRORLEVEL%