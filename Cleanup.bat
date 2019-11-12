@echo off
for /d /r . %%d in ("__pycache__") do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in ("imgui.ini") do @if exist "%%d" del /q "%%d"
exit