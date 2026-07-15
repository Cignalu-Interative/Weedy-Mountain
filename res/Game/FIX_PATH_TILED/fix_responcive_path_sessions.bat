@echo off
title Tiled Path Fixer
color 0A

:: Force l'execution depuis le dossier contenant le .bat
cd /d "%~dp0"

echo Lancement de l'outil de correction des chemins...
python fix_responcive_path_sessions.py

echo.
pause