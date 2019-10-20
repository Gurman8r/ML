@echo off
pushd %cd$\
cd ../
if exist ".\.vs" 	( rd /s /q .\.vs )
if exist ".\bin" 	( rd /s /q .\bin )
if exist ".\lib" 	( rd /s /q .\lib )
if exist ".\obj" 	( rd /s /q .\obj )
if exist ".\tmp" 	( rd /s /q .\tmp )
popd
exit