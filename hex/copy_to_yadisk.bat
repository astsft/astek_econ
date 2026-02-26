@echo off
set worspace_name=astek_econ
echo -------------------------------------------------------------------------
echo Workspace name: %worspace_name%
set year=%date:~-4%
set day=%date:~0,2%
set month=%date:~3,2%
set "todayName=%day%_%month%_%year%"
echo Date: %todayName%
echo -------------------------------------------------------------------------
set "workDir=C:\Users\Konstantin\YandexDisk\development\projects\astek\firmware\%worspace_name%\"
set "archDir=%workDir%archive\"

:: 1. Ищем последнюю созданную папку для архивации (кроме archive и сегодняшней)
for /f "delims=" %%d in ('dir /B /A:D /O:D "%workDir%"') do (
    if /I NOT "%%~d"=="archive" if /I NOT "%%~d"=="%todayName%" set "nameDir=%%~d"
)

:: 2. Архивируем и удаляем старую папку
if defined nameDir (
    echo Archiving: %nameDir%...
    if not exist "%archDir%" md "%archDir%"
    "C:\Program Files\WinRAR\WinRAR.exe" a -r -ep1 "%archDir%%nameDir%.rar" "%workDir%%nameDir%"
    if exist "%workDir%%nameDir%" rd /s /q "%workDir%%nameDir%"
)

:: 3. УДАЛЯЕМ новую папку, если она уже существует, и создаем чистую
if exist "%workDir%%todayName%" (
    echo Folder %todayName% exists. Deleting...
    rd /s /q "%workDir%%todayName%"
)
:: Небольшая пауза, чтобы файловая система успела освободить имя
timeout /t 1 >nul
md "%workDir%%todayName%"

for /l %%x in (0,1,2) do (
	echo Copy %worspace_name:~0,-2%%%x%worspace_name:~-1% hex:
	xcopy /s/e %worspace_name:~0,-2%%%x%worspace_name:~-1% %workDir%%day%_%month%_%year%\%worspace_name:~0,-2%%%x%worspace_name:~-1%\
	echo -------------------------------------------------------------------------
)
