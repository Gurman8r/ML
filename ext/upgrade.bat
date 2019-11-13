@echo off
git submodule sync
git submodule update --init --recursive
git pull
if %ERRORLEVEL% NEQ 0 ( pause )
