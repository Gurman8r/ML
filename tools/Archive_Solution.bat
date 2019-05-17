@echo off

rem Set Paths
set WorkingDir=%cd%/
set OutputDir=%WorkingDir%
set OutputFile=%OutputDir%\ML.zip

rem Generate Archive
cd C:\Program Files\7-Zip

7z a -r %OutputFile% %WorkingDir%../tools
7z a -r %OutputFile% %WorkingDir%../.git
7z a 	%OutputFile% %WorkingDir%../ML.sln
7z a 	%OutputFile% %WorkingDir%../ML_Config.ini
7z a 	%OutputFile% %WorkingDir%../*.bat
7z a 	%OutputFile% %WorkingDir%../README.md
7z a -r %OutputFile% %WorkingDir%../assets
7z a -r %OutputFile% %WorkingDir%../include
7z a -r %OutputFile% %WorkingDir%../plugins
7z a -r %OutputFile% %WorkingDir%../proj
7z a -r %OutputFile% %WorkingDir%../src
7z a -r %OutputFile% %WorkingDir%../thirdparty

rem Exit
cd %WorkingDir%
exit %ERRORLEVEL%