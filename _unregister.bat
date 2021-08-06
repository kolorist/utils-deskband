@echo off

call regsvr32 /u msvc/Debug/UtilsDeskBand.dll
taskkill /f /im explorer.exe
start explorer.exe