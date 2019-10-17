@echo off
if exist ".\lib"  rd /s /q .\lib )
if exist ".\obj"  rd /s /q .\obj )
for /d /r . %%d in ("__pycache__") do @IF exist "%%d" rd /s /q "%%d"