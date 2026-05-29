"""
find_duplicates.py
Analyse un repertoire recursivement et detecte les fichiers
portant le meme nom dans des dossiers differents.

Usage :
  python find_duplicates.py              -> analyse dossier courant
  python find_duplicates.py --rename     -> renomme les doublons : fichier_NomDossier.ext
"""

import os
import sys
import argparse
import shutil
from collections import defaultdict


def collect_files(root: str) -> dict[str, list[str]]:
    files_map: dict[str, list[str]] = defaultdict(list)
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            full_path = os.path.abspath(os.path.join(dirpath, filename))
            files_map[filename].append(full_path)
    return files_map


def find_duplicates(files_map: dict[str, list[str]]) -> dict[str, list[str]]:
    return {name: paths for name, paths in files_map.items() if len(paths) > 1}


def display_results(duplicates: dict[str, list[str]], root: str) -> None:
    if not duplicates:
        print("\n[OK] Aucun doublon de nom trouve.")
        return

    print(f"\n{'='*60}")
    print(f"  {len(duplicates)} nom(s) duplique(s) detecte(s)")
    print(f"{'='*60}\n")

    for name, paths in sorted(duplicates.items()):
        print(f"  >> {name}  ({len(paths)} occurrences)")
        for path in paths:
            rel = os.path.relpath(path, root)
            print(f"       {rel}")
        print()

    total = sum(len(p) for p in duplicates.values())
    print(f"{'─'*60}")
    print(f"  Total fichiers concernes : {total}")
    print(f"{'─'*60}\n")


def rename_duplicates(duplicates: dict[str, list[str]], root: str) -> None:
    """
    Pour chaque doublon, renomme chaque fichier en :
      nomfichier_NomDossierParent.ext
    Ne renomme pas si le nouveau nom existe deja.
    """
    if not duplicates:
        print("\n[OK] Aucun doublon a renommer.")
        return

    print(f"\n{'='*60}")
    print(f"  Renommage des doublons")
    print(f"{'='*60}\n")

    renamed = 0
    skipped = 0

    for name, paths in sorted(duplicates.items()):
        stem, ext = os.path.splitext(name)
        print(f"  >> {name}")

        for path in paths:
            parent_dir = os.path.basename(os.path.dirname(path))
            new_name = f"{stem}_{parent_dir}{ext}"
            new_path = os.path.join(os.path.dirname(path), new_name)

            rel_old = os.path.relpath(path, root)
            rel_new = os.path.relpath(new_path, root)

            if os.path.exists(new_path):
                print(f"       [SKIP] {rel_old}  ->  {new_name}  (existe deja)")
                skipped += 1
            else:
                os.rename(path, new_path)
                print(f"       [OK]   {rel_old}  ->  {new_name}")
                renamed += 1
        print()

    print(f"{'─'*60}")
    print(f"  Renommes : {renamed}   Ignores : {skipped}")
    print(f"{'─'*60}\n")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Detecte (et renomme) les fichiers en doublon de nom."
    )
    parser.add_argument(
        "--rename",
        action="store_true",
        help="Renomme les doublons en ajoutant le nom du dossier parent",
    )
    args = parser.parse_args()

    root = os.path.abspath(".")
    print(f"\n  Analyse de : {root}")

    files_map = collect_files(root)
    duplicates = find_duplicates(files_map)

    display_results(duplicates, root)

    if args.rename and duplicates:
        confirm = input("  Confirmer le renommage ? (o/n) : ").strip().lower()
        if confirm == "o":
            rename_duplicates(duplicates, root)
        else:
            print("  Renommage annule.\n")


if __name__ == "__main__":
    main()
