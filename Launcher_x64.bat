@echo off
cls

set Configuration=Release

set PlatformTarget=x64

set ExePath=.\bin\%Configuration%\%PlatformTarget%\

set ExeName=ML_Launcher_%Configuration%_%PlatformTarget%.exe

cd %ExePath%

start %ExeName% %*

if %ERRORLEVEL% NEQ 0 ( pause )