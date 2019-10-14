@echo off
rem Build Debug x64
cls
call Build_64.bat ^
 "%cd%\..\ML.sln" ^
 "/p:Configuration=Debug" ^
 "/p:PlatformTarget=x64" ^
 "/p:PlatformToolset=v142" ^
 "/p:Subsystem=Console"