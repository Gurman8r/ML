@echo off
rem Build Release x64
cls
call Build64.bat ^
 "%cd%\..\..\ML.sln" ^
 "/p:Configuration=Release" ^
 "/p:PlatformTarget=x64" ^
 "/p:PlatformToolset=v141"
exit /B %ERRORLEVEL%
