@echo off

set WorkingDir=%cd%/
set OutName=CppSandbox
set OutDir=%WorkingDir%
set OutType=.zip
set OutFile=%OutDir%%OutName%%OutType%

cd C:\Program Files\7-Zip

7z a -r %OutFile% %WorkingDir%../tools
7z a -r %OutFile% %WorkingDir%../.git
7z a 	%OutFile% %WorkingDir%../%OutName%.sln
7z a 	%OutFile% %WorkingDir%../ML_Config.ini
7z a 	%OutFile% %WorkingDir%../*.bat
7z a 	%OutFile% %WorkingDir%../README.md
7z a -r %OutFile% %WorkingDir%../assets
7z a -r %OutFile% %WorkingDir%../include
7z a -r %OutFile% %WorkingDir%../plugins
7z a -r %OutFile% %WorkingDir%../proj
7z a -r %OutFile% %WorkingDir%../src
7z a -r %OutFile% %WorkingDir%../thirdparty

exit %ERRORLEVEL%