@echo off
set worspace_name=astek_econ
set project1=astek_econ
set hw1=2041
set hw2=2331_rev3

set "workDir=G:\WORK\AST-SOFT\GAC-DEC-HW2217\firmware\gac_dec\%worspace_name%\iar\project\%worspace_name%"
echo -------------------------------------------------------------------------
echo Build ALL

echo Project: %project1%
echo Boad: %hw1%

echo #define ASBACK_HW2353REV3 > ../src/app/preconfig.h
"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe" %workDir%\%project1%.hw%hw1%.ewp release
"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe" %workDir%\%project1%.hw%hw2%.ewp release

echo #define EKON_2025 >> ../src/app/preconfig.h
"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe" %workDir%\%project1%.hw%hw1%.ewp release
"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe" %workDir%\%project1%.hw%hw2%.ewp release

echo #define ASBACK_HW2353REV5 > ../src/app/preconfig.h
"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe" %workDir%\%project1%.hw%hw2%.ewp release
echo #define EKON_2025 >> ../src/app/preconfig.h
"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe" %workDir%\%project1%.hw%hw2%.ewp release



