import os
import json
import sys

# ---------------------------------------------------------------------------
# EXTENSIONS PAR CATEGORIE
# On utilise des listes pour definir quels types de fichiers
# appartiennent a quelle categorie d'asset
# ---------------------------------------------------------------------------
EXTENSIONS_IMAGES = [".png"]
EXTENSIONS_MAPS   = [".tmx"]
EXTENSIONS_SONS   = [".mp3", ".wav"]
EXTENSIONS_FONTS  = [".ttf"]

# ---------------------------------------------------------------------------
# CHEMINS DU PROJET
# __file__             = chemin absolu de ce script
# os.path.abspath()    = s'assure que le chemin est absolu (pas relatif)
# os.path.dirname()    = remonte d'un niveau pour avoir le dossier parent
# ---------------------------------------------------------------------------
# Dossier ou se trouve ce script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Le script est dans res/2DPendentEngine/REPERTORY
# Chaque ".." remonte d'un niveau dans l'arborescence
# ".." x3 = on remonte jusqu'a la racine Moteur2DCignaluInteractive/
RACINE = os.path.join(SCRIPT_DIR, "..", "..", "..")

# Dossier de reference pour calculer les chemins relatifs
# C'est le dossier ou se trouve le .exe genere par Visual Studio
# Tous les chemins dans les JSON seront relatifs a ce dossier
DOSSIER_REFERENCE = os.path.join(RACINE, "ide", "Game")

# Dossier racine de tous les assets a scanner
DOSSIER_ASSETS = os.path.join(RACINE, "res", "2DPendentEngine")

# ---------------------------------------------------------------------------
# DOSSIERS DE SORTIE DES JSON
# Chaque categorie a son propre dossier de destination
# os.makedirs() les creera automatiquement s'ils n'existent pas encore
# ---------------------------------------------------------------------------
DOSSIER_JSON_TEXTURES = os.path.join(RACINE, "res", "2DPendentEngine", "Assets", "Textures")
DOSSIER_JSON_MAPS     = os.path.join(RACINE, "res", "2DPendentEngine", "Assets", "Maps")
DOSSIER_JSON_SOUNDS   = os.path.join(RACINE, "res", "2DPendentEngine", "Assets", "Sounds")
DOSSIER_JSON_FONTS    = os.path.join(RACINE, "res", "2DPendentEngine", "Assets", "Fonts")


def main():

    print("=== SCAN ASSETS - Cignalu Engine ===\n")

    # --- Validation des dossiers obligatoires ---
    # Si l'un de ces dossiers est introuvable, le script ne peut pas fonctionner
    if not os.path.isdir(DOSSIER_REFERENCE):
        print(f"[ERREUR] Dossier de reference introuvable : {DOSSIER_REFERENCE}")
        input("Appuie sur Entree pour fermer...")
        sys.exit(1)

    if not os.path.isdir(DOSSIER_ASSETS):
        print(f"[ERREUR] Dossier assets introuvable : {DOSSIER_ASSETS}")
        input("Appuie sur Entree pour fermer...")
        sys.exit(1)

    # --- Collections de chemins par categorie ---
    # On utilise des sets plutot que des listes pour eviter les doublons
    # Un set refuse automatiquement d'ajouter une valeur deja presente
    # C'est l'equivalent de std::unordered_set en C++
    chemins_textures = set()
    chemins_maps     = set()
    chemins_sons     = set()
    chemins_fonts    = set()

    # --- Scan recursif du dossier assets ---
    # os.walk() parcourt le dossier et tous ses sous-dossiers automatiquement
    # A chaque iteration :
    #   racine   = chemin du dossier en cours
    #   _        = sous-dossiers (on n'en a pas besoin, d'ou le "_")
    #   fichiers = liste des fichiers dans ce dossier
    for racine, _, fichiers in os.walk(DOSSIER_ASSETS):
        for fichier in fichiers:

            # os.path.splitext() separe le nom et l'extension
            # Exemple : "idle.png" -> ("idle", ".png")
            # Le "_" ignore le nom, on garde seulement l'extension
            _, ext = os.path.splitext(fichier)
            ext = ext.lower()  # .PNG et .png traites de la meme facon

            # Chemin absolu complet vers le fichier
            chemin_absolu = os.path.join(racine, fichier)

            # Chemin relatif depuis DOSSIER_REFERENCE
            # C'est ce chemin que le C++ utilisera pour charger le fichier
            # .replace() convertit les "\" de Windows en "/" pour la portabilite
            chemin_relatif = os.path.relpath(chemin_absolu, DOSSIER_REFERENCE).replace("\\", "/")

            # Tri dans la bonne categorie selon l'extension
            # .add() sur un set ignore silencieusement les doublons
            if ext in EXTENSIONS_IMAGES:
                chemins_textures.add(chemin_relatif)
            elif ext in EXTENSIONS_MAPS:
                chemins_maps.add(chemin_relatif)
            elif ext in EXTENSIONS_SONS:
                chemins_sons.add(chemin_relatif)
            elif ext in EXTENSIONS_FONTS:
                chemins_fonts.add(chemin_relatif)

    # --- Fonction locale d'ecriture JSON ---
    # Definie a l'interieur de main() pour avoir acces a ses variables
    # sans avoir besoin de les passer en parametre
    def ecrire_json(chemins, cle, nom_json, dossier_sortie):
        """
        Trie les chemins, cree le dossier de sortie si besoin,
        et ecrit le JSON final.

        chemins       : set de chemins relatifs a ecrire
        cle           : nom de la cle racine dans le JSON ("textures", "maps", "sounds")
        nom_json      : nom du fichier JSON a creer
        dossier_sortie: dossier ou le JSON sera ecrit
        """

        # sorted() convertit le set en liste triee alphabetiquement
        # Un set n'est pas ordonne, sorted() garantit un ordre stable
        chemins = sorted(chemins)

        print(f"-- {cle} : {len(chemins)} fichier(s) trouve(s) --")
        if chemins:
            print(f"  Exemple : {chemins[0]}")

        # Cree le dossier de sortie s'il n'existe pas
        # exist_ok=True = ne plante pas si le dossier existe deja
        os.makedirs(dossier_sortie, exist_ok=True)

        json_sortie = os.path.join(dossier_sortie, nom_json)

        # "with open(...) as f" = ouvre et ferme le fichier automatiquement (RAII)
        # "w"             = mode ecriture, ecrase le fichier s'il existe deja
        # encoding="utf-8"= supporte les caracteres speciaux dans les noms de fichiers
        with open(json_sortie, "w", encoding="utf-8") as f:
            # json.dump() ecrit le dictionnaire Python en JSON dans le fichier
            # indent=2         = indentation de 2 espaces pour la lisibilite
            # ensure_ascii=False = autorise les caracteres non-ASCII (accents etc.)
            json.dump({cle: [{"path": c} for c in chemins]}, f, indent=2, ensure_ascii=False)

        print(f"  [OK] {json_sortie}\n")

    # --- Generation des trois JSON ---
    ecrire_json(chemins_textures, "textures", "Textures_Repertory.json", DOSSIER_JSON_TEXTURES)
    ecrire_json(chemins_maps,     "maps",     "Maps_Repertory.json",     DOSSIER_JSON_MAPS)
    ecrire_json(chemins_sons,     "sounds",   "Sounds_Repertory.json",   DOSSIER_JSON_SOUNDS)
    ecrire_json(chemins_fonts,    "fonts",    "Fonts_Repertory.json",    DOSSIER_JSON_FONTS)

    print("\nScan termine !")
    input("Appuie sur Entree pour fermer...")


# ---------------------------------------------------------------------------
# POINT D'ENTREE
# __name__ vaut "__main__" uniquement quand le script est lance directement
# C'est l'equivalent du int main() en C++
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    main()