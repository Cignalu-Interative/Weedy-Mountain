import json
from pathlib import Path

# 1. Résolution dynamique des chemins
# Le script est dans : Weedy-Mountain/res/Game/FIX_PATH_TILED
script_dir = Path(__file__).resolve().parent

# On remente pour trouver la racine du repo (Weedy-Mountain)
repo_root = script_dir.parent.parent.parent
repo_root_posix = repo_root.as_posix() 

# Chemin exact vers ta session Tiled
session_file = script_dir.parent / "ProjetTiled" / "Weedy-Mountain.tiled-session"

def update_path(path_str):
    """
    Remplace tout ce qui précède '/res/Game/' par la racine actuelle du projet.
    """
    if isinstance(path_str, str) and "/res/Game/" in path_str:
        parts = path_str.split("/res/Game/", 1)
        return f"{repo_root_posix}/res/Game/{parts[1]}"
    return path_str

def make_path_responsive(data):
    """
    Parcourt récursivement le JSON pour corriger les chemins dans les clés et valeurs.
    """
    if isinstance(data, dict):
        new_dict = {}
        for key, value in data.items():
            new_key = update_path(key)
            new_dict[new_key] = make_path_responsive(value)
        return new_dict

    elif isinstance(data, list):
        return [make_path_responsive(item) for item in data]

    elif isinstance(data, str):
        return update_path(data)

    else:
        return data

def main():
    print("===========================================")
    print(" Tiled Session - Fix Responsive Paths")
    print("===========================================\n")

    if not session_file.exists():
        print(f"[ERREUR] Fichier introuvable : {session_file}")
        return

    # 1. Lecture du fichier
    try:
        with open(session_file, 'r', encoding='utf-8') as f:
            data = json.load(f)
    except Exception as e:
        print(f"[ERREUR] Impossible de lire ou parser le fichier JSON : {e}")
        print("Verifie que le fichier n'est pas vide ou corrompu.")
        return

    print(f"-> Racine actuelle detectee : {repo_root_posix}")
    print(f"-> Cible : {session_file.name}")
    
    # 2. Traitement des chemins
    updated_data = make_path_responsive(data)

    # 3. Validation de sécurité (on s'assure que le JSON généré est 100% valide)
    try:
        test_json = json.dumps(updated_data, indent=4, ensure_ascii=False)
        json.loads(test_json)  # Test de parsing rapide
    except Exception as e:
        print(f"[ERREUR CRITIQUE] Le JSON genere est invalide ! Annulation de la sauvegarde : {e}")
        return

    # 4. Sauvegarde sécurisée
    try:
        with open(session_file, 'w', encoding='utf-8') as f:
            f.write(test_json)
        print("\n[SUCCES] Tous les chemins ont ete mis a jour avec la racine actuelle !")
    except Exception as e:
        print(f"[ERREUR] Impossible d'ecrire dans le fichier : {e}")

if __name__ == "__main__":
    main()