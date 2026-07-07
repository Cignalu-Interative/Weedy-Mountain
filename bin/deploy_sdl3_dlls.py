"""
Script de déploiement des DLL SDL3 pour Cignalu Engine / Weedy Mountain.

Copie toutes les .dll trouvées dans le dossier source (lib/DLL3)
vers le dossier de destination (ide/Game), en ne recopiant que
les fichiers nouveaux ou modifiés (comparaison par date de modif + taille).
"""

import shutil
import sys
from pathlib import Path

# --- Configuration ---------------------------------------------------------
# Chemins relatifs à l'emplacement de ce script, qui vit dans bin/ :
#   Cignalu-Engine-2Dpendent/
#   ├── bin/
#   │   ├── deploy_sdl3_dlls.py   <- ici
#   │   └── deploy_sdl3_dlls.bat
#   ├── lib/DLL3/
#   └── ide/Game/

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent

SOURCE_DIR = REPO_ROOT / "lib" / "DLL3"
DEST_DIR = REPO_ROOT / "ide" / "Game"

# -----------------------------------------------------------------------


def needs_copy(src: Path, dst: Path) -> bool:
    """Retourne True si le fichier doit être copié (absent, taille ou date différente)."""
    if not dst.exists():
        return True
    src_stat = src.stat()
    dst_stat = dst.stat()
    if src_stat.st_size != dst_stat.st_size:
        return True
    if src_stat.st_mtime > dst_stat.st_mtime:
        return True
    return False


def main() -> int:
    print("=" * 60)
    print("Déploiement des DLL SDL3 - Cignalu Engine")
    print("=" * 60)

    if not SOURCE_DIR.exists():
        print(f"[ERREUR] Dossier source introuvable : {SOURCE_DIR}")
        return 1

    if not DEST_DIR.exists():
        print(f"[INFO] Dossier de destination introuvable, création : {DEST_DIR}")
        DEST_DIR.mkdir(parents=True, exist_ok=True)

    dll_files = sorted(SOURCE_DIR.glob("*.dll"))

    if not dll_files:
        print(f"[ATTENTION] Aucune .dll trouvée dans {SOURCE_DIR}")
        return 1

    print(f"\n{len(dll_files)} DLL trouvée(s) dans le dossier source :\n")

    copied = 0
    skipped = 0

    for dll in dll_files:
        dest_path = DEST_DIR / dll.name

        if needs_copy(dll, dest_path):
            shutil.copy2(dll, dest_path)
            print(f"  [COPIÉ]   {dll.name}")
            copied += 1
        else:
            print(f"  [À JOUR]  {dll.name}")
            skipped += 1

    print("\n" + "-" * 60)
    print(f"Terminé : {copied} copiée(s), {skipped} déjà à jour.")
    print("-" * 60)

    return 0


if __name__ == "__main__":
    exit_code = main()
    sys.exit(exit_code)
