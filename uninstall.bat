@echo off
if exist ".\bin" 		( rd /s /q .\bin )
if exist ".\lib" 		( rd /s /q .\lib )
if exist ".\obj" 		( rd /s /q .\obj )
if exist ".\proj" 		( rd /s /q .\proj )
if exist ".\tmp" 		( rd /s /q .\tmp )
if exist ".\bin-lib"	( rd /s /q .\bin-lib )
if exist ".\bin-obj"	( rd /s /q .\bin-obj )
if exist ".\ML.sln" 	( del .\ML.sln )
FOR /d /r . %%d IN ("__pycache__") DO @IF EXIST "%%d" rd /s /q "%%d"
exit