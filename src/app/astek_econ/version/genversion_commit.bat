@ECHO OFF
SET mypath=%~dp0
echo %mypath%

echo %DATE% %TIME%

rm %mypath%version.c

for /f "tokens=*" %%i in ('git show -s --pretty^=format:"%%h" HEAD') do set S=%%i
echo COMMIT SHA : %S%

set T=%time: =0%

set datetimef=%date:~-2%%date:~3,2%%date:~0,2%_%T:~0,2%%T:~3,2%%T:~6,2%

echo #include "version.h" > %mypath%version.c

echo //>> %mypath%version.c
echo // Generated on %D% %time%>> %mypath%version.c
echo //   by %username%>> %mypath%version.c
echo //>> %mypath%version.c

echo #ifdef NDEBUG>> %mypath%version.c 
echo #define _CONFIG_STR_ "Release">> %mypath%version.c
echo #else>> %mypath%version.c 
echo #define _CONFIG_STR_ "Debug">> %mypath%version.c
echo #endif>> %mypath%version.c
echo uint8_t* firmware_commit =(uint8_t*)"%S%";>> %mypath%version.c
echo uint8_t* version = (uint8_t*)_CONFIG_STR_;>> %mypath%version.c
echo uint8_t* built_date = (uint8_t*) __DATE__;>> %mypath%version.c
echo uint8_t* built_time = (uint8_t*) __TIME__;>> %mypath%version.c
echo uint8_t* built_date_time = (uint8_t*) "%datetimef%";>> %mypath%version.c

