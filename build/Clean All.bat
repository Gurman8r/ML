@echo off
pushd %cd$\
cd ../
if exist ".\bin" 		( rd /s /q .\bin )
if exist ".\lib" 		( rd /s /q .\lib )
if exist ".\obj" 		( rd /s /q .\obj )
if exist ".\bin-lib"	( rd /s /q .\bin-lib )
if exist ".\bin-obj"	( rd /s /q .\bin-obj )
popd
exit