"""
================================================================================
 fix_tiled_paths.py
================================================================================

BUT DU SCRIPT
--------------
Tiled (l'editeur de cartes) ecrit les chemins des balises <tileset source="...">
(dans les .tmx) et <image source="..."> (dans les .tsx) de facon RELATIVE
au fichier lui-meme.

Exemple :
    Fichier : res/Game/Assets/Maps/tmx/DemoMap.tmx
    Contient : <tileset source="../tsx/world_tileset.tsx"/>
    -> "../tsx/" veut dire : depuis le dossier du .tmx, remonter d'un niveau
       puis aller dans "tsx". Ca fonctionne tres bien... mais SEULEMENT si
       on resout le chemin depuis l'EMPLACEMENT DU FICHIER .tmx.

Le probleme : ton moteur C++ (l'executable Visual Studio) ne resout PAS les
chemins depuis le .tmx. Il resout les chemins depuis le dossier ou se trouve
l'executable (working directory), qui est ide/Game/. Le chemin relatif ecrit
par Tiled ne correspond donc plus a rien une fois charge par le moteur.

CE QUE FAIT CE SCRIPT
----------------------
Il convertit les chemins dans les deux sens :

  MODE "-tiled"  : remet des chemins relatifs CORRECTS pour que Tiled puisse
                    ouvrir/editer la map sans erreur de tileset/texture manquant.
                    (chemin relatif depuis le fichier .tmx ou .tsx lui-meme)

  MODE "-visual" : remet des chemins au format attendu par le moteur C++,
                    relatifs a l'executable Visual Studio (ide/Game/), au
                    format "../../res/Game/Assets/...".

Le script est IDEMPOTENT : tu peux le lancer plusieurs fois dans le meme mode,
le resultat ne change pas / ne casse rien.

ARBORESCENCE DE REFERENCE (chez Alexis)
-----------------------------------------
    ide/Game/                                  <- l'executable Visual Studio tourne ici
    res/Game/ProjetTiled/                      <- projet Tiled (.tiled-project)
    res/Game/Assets/Maps/tmx/*.tmx              <- fichiers maps
    res/Game/Assets/Maps/tsx/*.tsx              <- fichiers tilesets
    res/Game/Assets/Textures/<sous-dossiers>/*.png  <- images sources des tilesets

Exemples de chemins attendus dans chaque mode :
    MODE TILED  (depuis Maps/tmx/DemoMap.tmx)      -> "../tsx/world_tileset.tsx"
    MODE TILED  (depuis Maps/tsx/world_tileset.tsx) -> "../../Textures/world_tileset.png"
    MODE VISUAL (depuis ide/Game/, pour le .tmx)    -> "../../res/Game/Assets/Maps/tmx/DemoMap.tmx"
    MODE VISUAL (depuis ide/Game/, pour le .tsx)    -> "../../res/Game/Assets/Maps/tsx/world_tileset.tsx"
    MODE VISUAL (depuis ide/Game/, pour la texture) -> "../../res/Game/Assets/Textures/<sous-dossier>/world_tileset.png"

UTILISATION
-----------
    python fix_tiled_paths.py -tiled
    python fix_tiled_paths.py -visual

Le script traite TOUS les .tmx dans Maps/tmx/ et TOUS les .tsx dans Maps/tsx/.
"""

import os
import sys
import re
import xml.etree.ElementTree as ET

# ==============================================================================
# 1. CONFIGURATION - CHEMIN RACINE EN DUR
# ==============================================================================
# Ce script (.py) est toujours place au meme endroit fixe par rapport a la
# racine du projet. On definit ici, en dur, le chemin DEPUIS CE SCRIPT vers
# la racine du repo (le dossier qui contient a la fois "ide" et "res").
#
# Si tu deplaces le script, c'est CETTE ligne qu'il faut corriger.
#
# Ici on suppose : <RACINE>/ide/Game/  et  <RACINE>/res/Game/
# Si le script est appele depuis un .bat place dans ide/Game/, alors la
# racine du repo est juste 2 dossiers au-dessus du script.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Chemin EN DUR vers la racine du repo (le dossier parent commun a "ide" et "res").
# A MODIFIER ICI si l'emplacement du script change un jour.
REPO_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "..", ".."))

# Dossiers reels sur le disque (toujours les memes, qu'on soit en mode
# -tiled ou -visual : on lit/ecrit toujours les MEMES fichiers physiques,
# seul le CONTENU des balises source="..." change).
MAPS_TMX_DIR = os.path.join(REPO_ROOT, "res", "Game", "Assets", "Maps", "tmx")
MAPS_TSX_DIR = os.path.join(REPO_ROOT, "res", "Game", "Assets", "Maps", "tsx")
TEXTURES_DIR = os.path.join(REPO_ROOT, "res", "Game", "Assets", "Textures")

# Prefixe a utiliser en mode "-visual" pour atteindre res/Game/ depuis
# l'executable qui tourne dans ide/Game/.
# ide/Game/  ->  ../../res/Game/   (on remonte 2 fois : Game/ puis ide/)
VISUAL_PREFIX = "../../res/Game"


# ==============================================================================
# 2. FONCTIONS UTILITAIRES DE CHEMINS
# ==============================================================================

def resolve_real_path(file_path, source_value):
    """
    Resout le chemin REEL sur le disque pointe par la valeur d'une balise
    source="..." trouvee dans 'file_path'.

    Peu importe si 'source_value' est actuellement au format Tiled
    (ex: "../tsx/world_tileset.tsx") ou au format Visual
    (ex: "../../res/Game/Assets/Maps/tsx/world_tileset.tsx") : dans les
    deux cas, on resout le chemin RELATIF par rapport au dossier qui
    contient 'file_path', et on obtient un chemin ABSOLU reel.

    C'est cette etape qui permet au script de fonctionner peu importe le
    format de depart : on retombe toujours sur le meme fichier physique.
    """
    file_dir = os.path.dirname(file_path)
    candidate = os.path.normpath(os.path.join(file_dir, source_value))
    return candidate


def find_real_file_for_basename(start_dir_options, filename):
    """
    Filet de securite : si resolve_real_path() ne tombe pas exactement sur
    un fichier existant (par exemple parce que le chemin source="..." actuel
    est invalide/casse des deux cotes), on cherche le fichier par son NOM
    dans une liste de dossiers candidats (Maps/tmx, Maps/tsx, Textures et
    ses sous-dossiers).

    Retourne le chemin absolu trouve, ou None si rien n'est trouve.
    """
    for base_dir in start_dir_options:
        if not os.path.isdir(base_dir):
            continue
        for root, _dirs, files in os.walk(base_dir):
            if filename in files:
                return os.path.join(root, filename)
    return None


def to_posix(path):
    """
    Tiled (et la plupart des moteurs) attendent des slashs "/" dans les
    chemins, meme sous Windows. Cette fonction convertit les backslashs
    Windows en slashs Unix pour l'ecriture dans les fichiers XML.
    """
    return path.replace("\\", "/")


def compute_relative_path(from_file, to_file):
    """
    Calcule le chemin relatif pour aller du dossier de 'from_file' vers
    le fichier 'to_file'. Utilise pour le MODE TILED (chemin relatif
    correct depuis le fichier .tmx/.tsx lui-meme).
    """
    from_dir = os.path.dirname(from_file)
    rel = os.path.relpath(to_file, from_dir)
    return to_posix(rel)


def compute_visual_path(real_abs_path):
    """
    Calcule le chemin au format MODE VISUAL pour un fichier reel donne
    (chemin absolu sur le disque).

    Le format vise est toujours :
        ../../res/Game/<le reste du chemin depuis res/Game/>

    On ne devine pas de structure : on prend le chemin reel du fichier,
    on retire la partie commune jusqu'a "res/Game/", et on recolle
    derriere VISUAL_PREFIX. Ca marche pour les .tmx, les .tsx, ET les
    textures (peu importe leur sous-dossier sous Assets/Textures/).
    """
    res_game_root = os.path.join(REPO_ROOT, "res", "Game")
    rel_from_res_game = os.path.relpath(real_abs_path, res_game_root)
    visual_path = VISUAL_PREFIX + "/" + to_posix(rel_from_res_game)
    return visual_path


# ==============================================================================
# 3. TRAITEMENT D'UN FICHIER .TSX (TILESET)
#    -> contient une balise <image source="..."/> qui pointe vers le PNG
# ==============================================================================

def process_tsx(tsx_path, mode):
    """
    Ouvre un fichier .tsx, trouve la balise <image source="...">, calcule
    le nouveau chemin selon le mode demande, et reecrit le fichier.

    On utilise une regex plutot qu'un parsing XML complet en ecriture,
    car ElementTree reformate parfois le XML (perte de commentaires,
    changement de l'ordre des attributs, etc.) ce qui peut perturber
    Tiled. La regex cible precisement l'attribut source="..." de la
    balise <image, sans toucher au reste du fichier.
    """
    with open(tsx_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Regex qui capture : <image ... source="VALEUR" ...
    # Le groupe 1 = tout ce qui est avant la valeur (pour la reecrire a l'identique)
    # Le groupe 2 = la valeur du chemin actuel (ce qu'on va remplacer)
    pattern = re.compile(r'(<image[^>]*\bsource=")([^"]+)(")')

    match = pattern.search(content)
    if not match:
        print(f"  [ATTENTION] Aucune balise <image source=\"...\"> trouvee dans {tsx_path}")
        return False

    current_source = match.group(2)

    # Etape 1 : retrouver le fichier PNG REEL sur le disque, peu importe
    # le format actuel du chemin (Tiled ou Visual, casse ou pas).
    real_png_path = resolve_real_path(tsx_path, current_source)

    if not os.path.isfile(real_png_path):
        # Le chemin actuel ne mene a rien -> on cherche le PNG par son nom
        # dans le dossier Textures (et ses sous-dossiers) en filet de securite.
        png_filename = os.path.basename(current_source)
        real_png_path = find_real_file_for_basename([TEXTURES_DIR], png_filename)
        if real_png_path is None:
            print(f"  [ERREUR] Impossible de localiser la texture '{png_filename}' "
                  f"reference par {tsx_path}")
            return False

    real_png_path = os.path.abspath(real_png_path)

    # Etape 2 : calculer le nouveau chemin selon le mode demande.
    if mode == "tiled":
        new_source = compute_relative_path(tsx_path, real_png_path)
    else:  # mode == "visual"
        new_source = compute_visual_path(real_png_path)

    # Etape 3 : reecrire uniquement la valeur de l'attribut source, en
    # gardant tout le reste du fichier identique.
    new_content = content[:match.start(2)] + new_source + content[match.end(2):]

    with open(tsx_path, "w", encoding="utf-8") as f:
        f.write(new_content)

    print(f"  [OK] {os.path.basename(tsx_path)} : image source -> {new_source}")
    return True


# ==============================================================================
# 4. TRAITEMENT D'UN FICHIER .TMX (MAP)
#    -> peut contenir PLUSIEURS balises <tileset source="...">
#       et PLUSIEURS balises <image source="..."> dans des <imagelayer>
# ==============================================================================

def process_tmx(tmx_path, mode):
    """
    Ouvre un fichier .tmx, trouve TOUTES les balises <tileset source="...">
    (un .tmx peut utiliser plusieurs tilesets) ET toutes les balises
    <image source="..."> situees dans des <imagelayer> (calques d'image,
    differents des tilesets), calcule les nouveaux chemins, et reecrit
    le fichier.
    """
    with open(tmx_path, "r", encoding="utf-8") as f:
        content = f.read()

    modified = False

    # --------------------------------------------------------------------
    # 4.1 - Balises <tileset source="...">  (reference vers un .tsx)
    # --------------------------------------------------------------------
    # On ignore les <tileset> qui n'ont PAS d'attribut "source" : un
    # tileset peut etre defini directement "en dur" dans le .tmx (embedded),
    # dans ce cas il n'y a pas de fichier externe a corriger.
    tileset_pattern = re.compile(r'(<tileset[^>]*\bsource=")([^"]+)(")')

    def replace_tileset(match):
        nonlocal modified
        current_source = match.group(2)
        real_tsx_path = resolve_real_path(tmx_path, current_source)

        if not os.path.isfile(real_tsx_path):
            tsx_filename = os.path.basename(current_source)
            real_tsx_path = find_real_file_for_basename([MAPS_TSX_DIR], tsx_filename)
            if real_tsx_path is None:
                print(f"  [ERREUR] Impossible de localiser le tileset '{tsx_filename}' "
                      f"reference par {tmx_path}")
                return match.group(0)  # on ne touche pas a la ligne si on ne trouve rien

        real_tsx_path_abs = os.path.abspath(real_tsx_path)

        if mode == "tiled":
            new_source = compute_relative_path(tmx_path, real_tsx_path_abs)
        else:
            new_source = compute_visual_path(real_tsx_path_abs)

        modified = True
        print(f"  [OK] {os.path.basename(tmx_path)} : tileset source -> {new_source}")
        return match.group(1) + new_source + match.group(3)

    content = tileset_pattern.sub(replace_tileset, content)

    # --------------------------------------------------------------------
    # 4.2 - Balises <image source="..."> a l'interieur des <imagelayer>
    # --------------------------------------------------------------------
    # Un imagelayer ressemble a :
    #   <imagelayer ...>
    #     <image source="../../Textures/background.png" .../>
    #   </imagelayer>
    # On ne veut PAS toucher a d'autres balises <image> qui pourraient
    # exister ailleurs (normalement il n'y en a pas dans un .tmx hors
    # imagelayer, mais on isole bien le bloc par securite).
    imagelayer_block_pattern = re.compile(r'(<imagelayer\b.*?</imagelayer>|<imagelayer\b[^>]*/>)', re.DOTALL)
    image_in_block_pattern = re.compile(r'(<image[^>]*\bsource=")([^"]+)(")')

    def replace_image_in_imagelayer(block_match):
        nonlocal modified
        block_text = block_match.group(0)

        def replace_image(match):
            nonlocal modified
            current_source = match.group(2)
            real_img_path = resolve_real_path(tmx_path, current_source)

            if not os.path.isfile(real_img_path):
                img_filename = os.path.basename(current_source)
                real_img_path = find_real_file_for_basename([TEXTURES_DIR], img_filename)
                if real_img_path is None:
                    print(f"  [ERREUR] Impossible de localiser l'image '{img_filename}' "
                          f"(imagelayer) reference par {tmx_path}")
                    return match.group(0)

            real_img_path_abs = os.path.abspath(real_img_path)

            if mode == "tiled":
                new_source = compute_relative_path(tmx_path, real_img_path_abs)
            else:
                new_source = compute_visual_path(real_img_path_abs)

            modified = True
            print(f"  [OK] {os.path.basename(tmx_path)} : imagelayer image source -> {new_source}")
            return match.group(1) + new_source + match.group(3)

        return image_in_block_pattern.sub(replace_image, block_text)

    content = imagelayer_block_pattern.sub(replace_image_in_imagelayer, content)

    if modified:
        with open(tmx_path, "w", encoding="utf-8") as f:
            f.write(content)

    return modified


# ==============================================================================
# 5. POINT D'ENTREE DU SCRIPT
# ==============================================================================

def main():
    # ----- 5.1 Lecture et validation de l'argument (-tiled / -visual) -------
    if len(sys.argv) != 2 or sys.argv[1] not in ("-tiled", "-visual"):
        print("Utilisation : python fix_tiled_paths.py -tiled|-visual")
        print("  -tiled  : remet des chemins relatifs corrects pour ouvrir/editer dans Tiled")
        print("  -visual : remet des chemins au format attendu par le moteur C++ (Visual Studio)")
        sys.exit(1)

    mode = "tiled" if sys.argv[1] == "-tiled" else "visual"

    print(f"=== Conversion des chemins Tiled -> mode '{mode.upper()}' ===")
    print(f"Racine du repo detectee : {REPO_ROOT}")
    print(f"Dossier maps tmx        : {MAPS_TMX_DIR}")
    print(f"Dossier maps tsx        : {MAPS_TSX_DIR}")
    print(f"Dossier textures        : {TEXTURES_DIR}")
    print()

    # ----- 5.2 Verification que les dossiers attendus existent bien --------
    if not os.path.isdir(MAPS_TMX_DIR):
        print(f"[ERREUR] Dossier introuvable : {MAPS_TMX_DIR}")
        sys.exit(1)
    if not os.path.isdir(MAPS_TSX_DIR):
        print(f"[ERREUR] Dossier introuvable : {MAPS_TSX_DIR}")
        sys.exit(1)

    # ----- 5.3 Traitement de tous les .tsx (tilesets) -----------------------
    print("--- Traitement des fichiers .tsx (tilesets) ---")
    tsx_files = [f for f in os.listdir(MAPS_TSX_DIR) if f.lower().endswith(".tsx")]
    if not tsx_files:
        print("  Aucun fichier .tsx trouve.")
    for tsx_name in sorted(tsx_files):
        tsx_path = os.path.join(MAPS_TSX_DIR, tsx_name)
        process_tsx(tsx_path, mode)

    print()

    # ----- 5.4 Traitement de tous les .tmx (maps) ---------------------------
    print("--- Traitement des fichiers .tmx (maps) ---")
    tmx_files = [f for f in os.listdir(MAPS_TMX_DIR) if f.lower().endswith(".tmx")]
    if not tmx_files:
        print("  Aucun fichier .tmx trouve.")
    for tmx_name in sorted(tmx_files):
        tmx_path = os.path.join(MAPS_TMX_DIR, tmx_name)
        process_tmx(tmx_path, mode)

    print()
    print(f"=== Termine. Tous les chemins sont maintenant au format '{mode.upper()}'. ===")


if __name__ == "__main__":
    main()
