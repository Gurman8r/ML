@echo off
rem Build Release x64
cls
call Build_64.bat ^
 "%cd%\..\..\ML.sln" ^
 "/p:Configuration=Release" ^
 "/p:PlatformTarget=x64" ^
 "/p:PlatformToolset=v141" ^
 "/p:Subsystem=Windows"
exit /B %ERRORLEVEL%
