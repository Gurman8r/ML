@echo off
rem Build Debug x64
cls
call Build_64.bat ^
 "%cd%\..\..\ML.sln" ^
 "/p:Configuration=Debug" ^
 "/p:PlatformTarget=x64" ^
 "/p:PlatformToolset=v141" ^
 "/p:Subsystem=Console"
exit /B %ERRORLEVEL%