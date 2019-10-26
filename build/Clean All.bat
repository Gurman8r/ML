@echo off
pushd %cd$\
cd ../
if exist ".\bin" 	( rd /s /q .\bin )
if exist ".\lib" 	( rd /s /q .\lib )
if exist ".\obj" 	( rd /s /q .\obj )
popd
exit