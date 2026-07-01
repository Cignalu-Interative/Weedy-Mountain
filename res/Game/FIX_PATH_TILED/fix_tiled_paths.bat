@echo off
REM ============================================================================
REM  fix_tiled_paths.bat
REM ============================================================================
REM  But : lancer facilement le script Python fix_tiled_paths.py sans avoir a
REM        taper la commande python a la main, et sans avoir a se souvenir
REM        du mot-cle exact (-tiled ou -visual).
REM
REM  Ce .bat doit rester dans le MEME dossier que fix_tiled_paths.py
REM  (le script Python est appele via un chemin relatif a ce .bat).
REM ============================================================================

REM %~dp0 = chemin absolu du dossier qui contient CE fichier .bat
REM (avec un slash a la fin). On s'en sert pour appeler le .py a cote,
REM peu importe depuis quel dossier on double-clique/lance le .bat.
set SCRIPT_DIR=%~dp0
set PY_SCRIPT=%SCRIPT_DIR%fix_tiled_paths.py

REM --- Si un argument a deja ete donne au .bat (ex: appel automatise) -------
REM Permet aussi de faire : fix_tiled_paths.bat -tiled  en ligne de commande,
REM sans passer par le menu.
if "%~1"=="-tiled" goto :run_tiled
if "%~1"=="-visual" goto :run_visual

REM --- Sinon on affiche un petit menu interactif -----------------------------
:menu
cls
echo ============================================================
echo   Correction des chemins Tiled (.tmx / .tsx)
echo ============================================================
echo.
echo   Choisis le mode de conversion :
echo.
echo   [1] -tiled    Chemins relatifs corrects pour ouvrir/editer
echo                  les maps dans l'editeur Tiled
echo.
echo   [2] -visual   Chemins au format attendu par le moteur C++
echo                  (executable Visual Studio dans ide/Game)
echo.
echo   [0] Quitter
echo.
set /p CHOIX="Ton choix (0/1/2) : "

if "%CHOIX%"=="1" goto :run_tiled
if "%CHOIX%"=="2" goto :run_visual
if "%CHOIX%"=="0" goto :end

echo.
echo Choix invalide, reessaie.
pause
goto :menu

REM --- Execution mode Tiled ---------------------------------------------------
:run_tiled
echo.
echo Lancement en mode -tiled ...
echo.
python "%PY_SCRIPT%" -tiled
goto :after_run

REM --- Execution mode Visual Studio --------------------------------------------
:run_visual
echo.
echo Lancement en mode -visual ...
echo.
python "%PY_SCRIPT%" -visual
goto :after_run

REM --- Apres execution : on affiche le resultat et on attend avant de fermer --
:after_run
echo.
echo ------------------------------------------------------------
echo Operation terminee. Verifie les messages [OK] / [ERREUR] ci-dessus.
echo ------------------------------------------------------------
pause

:end
