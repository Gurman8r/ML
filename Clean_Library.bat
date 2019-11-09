@echo off
for /d /r . %%d in ("__pycache__") do @if exist "%%d" rd /s /q "%%d"
exit