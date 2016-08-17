ECHO OFF 

ECHO Install 32 bit
c:\windows\syswow64\rundll32.exe setupapi.dll,InstallHinfSection DefaultInstall 0 %~dp0gfwx32.inf

pause
