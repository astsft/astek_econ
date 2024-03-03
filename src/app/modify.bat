@echo off
SET mypath=%~dp0
rem echo %mypath%

SET filename=modify_hex_name.ps1
rem echo %filename%

SET filepath=%mypath%%filename%
rem echo %filepath%

powershell -file %filepath%
