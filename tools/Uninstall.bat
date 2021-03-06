@echo off

if exist ..\.vs rd /s /q ..\.vs
if exist ..\bin rd /s /q ..\bin
if exist ..\bin-lib rd /s /q ..\bin-lib
if exist ..\bin-obj rd /s /q ..\bin-obj
if exist ..\proj rd /s /q ..\proj

if exist ..\ML_vs2017.sln del /q ..\ML_vs2017.sln
if exist ..\ML_vs2019.sln del /q ..\ML_vs2019.sln

for /d /r ..\ %%d in ("__pycache__") do @if exist "%%d" rd /s /q "%%d"
for /d /r ..\ %%d in ("imgui.ini") do @if exist "%%d" del /q "%%d"

if %ERRORLEVEL% NEQ 0 ( pause )