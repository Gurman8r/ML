@echo off
rem Build Release x86
cls
call Build_32.bat ^
 "%cd%\..\ML.sln" ^
 "/p:Configuration=Release" ^
 "/p:PlatformTarget=x86" ^
 "/p:PlatformToolset=v142" ^
 "/p:Subsystem=Windows"