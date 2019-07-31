@echo off
rem Build Release x86
cls
call Build32.bat ^
 "%cd%\..\ML.sln" ^
 "/p:Configuration=Release" ^
 "/p:PlatformTarget=x86" ^
 "/p:PlatformToolset=v141"
exit %ERRORLEVEL%