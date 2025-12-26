@echo off
set worspace_name=astek_econ
echo -------------------------------------------------------------------------
echo Workspace name: %worspace_name%
set year=%date:~-4%
set day=%date:~0,2%
set month=%date:~3,2%
echo Date: %day%_%month%_%year%
echo -------------------------------------------------------------------------
set "workDir=C:\Users\Konstantin\YandexDisk\development\projects\astek\firmware\%worspace_name%\"

for /f %%d in ('dir /B /A:D %workDir%') do set "nameDir=%%~d"
"C:\Program Files\WinRAR\WinRAR.exe" a -r %workDir%%nameDir%.rar %workDir%%nameDir%

RD /s/q %workDir%%day%_%month%_%year%
MD %workDir%%day%_%month%_%year%

for /l %%x in (0,1,2) do (
	echo Copy %worspace_name:~0,-2%%%x%worspace_name:~-1% hex:
	xcopy /s/e %worspace_name:~0,-2%%%x%worspace_name:~-1% %workDir%%day%_%month%_%year%\%worspace_name:~0,-2%%%x%worspace_name:~-1%\
	echo -------------------------------------------------------------------------
)