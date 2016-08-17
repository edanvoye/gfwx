ECHO OFF 

ECHO Install 64 bit
rundll32.exe setupapi.dll,InstallHinfSection DefaultInstall 0 %~dp0gfwx64.inf

pause
