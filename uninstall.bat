@echo off

if exist .\bin rd /s /q .\bin

if exist .\bin-lib rd /s /q .\bin-lib

if exist .\bin-obj rd /s /q .\bin-obj

if exist .\proj rd /s /q .\proj

del /s /q /f .\*.sln

for /d /r . %%d in ("__pycache__") do @if exist "%%d" rd /s /q "%%d"

if %ERRORLEVEL% NEQ 0 ( pause )