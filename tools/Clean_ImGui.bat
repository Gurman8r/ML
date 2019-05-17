@echo off & setlocal

rem Delete all imgui INI files

cd ..\
del /S imgui.ini

exit %ERRORLEVEL%