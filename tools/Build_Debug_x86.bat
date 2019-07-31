@echo off
rem Build Debug x86
cls
call Build32.bat ^
 "%cd%\..\ML.sln" ^
 "/p:Configuration=Debug" ^
 "/p:PlatformTarget=x86" ^
 "/p:PlatformToolset=v141"
exit %ERRORLEVEL%