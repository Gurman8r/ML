@echo off
if exist ".\.vs" 			( rd /s /q .\.vs )
if exist ".\bin" 			( rd /s /q .\bin )
if exist ".\lib" 			( rd /s /q .\lib )
if exist ".\obj" 			( rd /s /q .\obj )
if exist ".\proj" 			( rd /s /q .\proj )
if exist ".\tmp" 			( rd /s /q .\tmp )
if exist ".\ML.sln" 		( del .\ML.sln )
if exist ".\ML.workspace" 	( del .\ML.workspace )
exit