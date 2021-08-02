@echo off

call regsvr32 /u clang64/UtilsDeskBand.dll
taskkill /f /im explorer.exe
start explorer.exe