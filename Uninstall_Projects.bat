@echo off

if exist .\.vs rd /s /q .\.vs

if exist .\proj rd /s /q .\proj

del /s /q /f .\*.sln

if %ERRORLEVEL% NEQ 0 ( pause )