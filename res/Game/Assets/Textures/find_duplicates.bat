@echo off
setlocal EnableDelayedExpansion

where python >nul 2>&1
if errorlevel 1 (
    echo [ERREUR] Python n'est pas installe ou pas dans le PATH.
    pause
    exit /b 1
)

set "SCRIPT_DIR=%~dp0"
set "PY_SCRIPT=%SCRIPT_DIR%find_duplicates.py"

if not exist "%PY_SCRIPT%" (
    echo [ERREUR] find_duplicates.py introuvable dans %SCRIPT_DIR%
    pause
    exit /b 1
)

:MENU
cls
echo.
echo ============================================
echo      Detecteur de doublons de fichiers
echo ============================================
echo.
echo [1]  Analyser les doublons
echo [2]  Renommer les doublons (ajoute le nom du dossier)
echo [3]  Quitter
echo.
set /p "CHOICE=Votre choix : "

if "!CHOICE!"=="1" goto ANALYSE
if "!CHOICE!"=="2" goto RENAME
if "!CHOICE!"=="3" exit /b 0

echo Choix invalide.
timeout /t 1 >nul
goto MENU

:ANALYSE
echo.
python "%PY_SCRIPT%"
goto END

:RENAME
echo.
python "%PY_SCRIPT%" --rename
goto END

:END
echo.
pause
endlocal
