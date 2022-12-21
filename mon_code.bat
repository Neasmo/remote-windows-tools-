@echo off

rem Demande le nom de l'ordinateur, le nom d'utilisateur et le mot de passe à l'utilisateur
set /p computerName=Entrez le nom de l'ordinateur :
set /p username=Entrez le nom d'utilisateur :
set /p password=Entrez le mot de passe :

rem Récupère la liste des utilisateurs de l'ordinateur distant
for /f "tokens=*" %%a in ('
runas /user:%computerName%%username% %password% "cmd.exe /c wmic useraccount get name"
') do (
rem Si l'utilisateur est "Public", "DefaultAccount" ou "DefaultAppPool", on ignore
if /i "%%a"=="Public" goto skip
if /i "%%a"=="DefaultAccount" goto skip
if /i "%%a"=="DefaultAppPool" goto skip

    rem Exécute la commande "cmd.exe /c del /q /f /s %USERPROFILE%\AppData\Local\Temp\*.*" pour supprimer les fichiers temporaires de l'utilisateur
    runas /user:%computerName%\%username% %password% "cmd.exe /c del /q /f /s %USERPROFILE%\AppData\Local\Temp\*.*"

    rem Exécute la commande "cmd.exe /c rd /q /s %USERPROFILE%\Desktop\Recycle Bin\*.*" pour vider la corbeille de l'utilisateur
    runas /user:%computerName%\%username% %password% "cmd.exe /c rd /q /s %USERPROFILE%\Desktop\Recycle Bin\*.*"

:skip
)

rem Exécute la commande "ipconfig /flushdns" pour vider le cache DNS
runas /user:%computerName%\%username% %password% "cmd.exe /c ipconfig /flushdns"

pause
