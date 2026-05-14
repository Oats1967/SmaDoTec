copy EasyControl_FRZ.org EasyControl_FRZ.rca
rem txtcnv32.exe -EasyControl_FRZ.rc -d:65001 -e:1252
CONVERTCP 65001 0 /i "EasyControl_FRZ.rca" /o "EasyControl_FRZ.rc"
