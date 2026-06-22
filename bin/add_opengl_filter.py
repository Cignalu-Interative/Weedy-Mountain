#!/usr/bin/env python3
"""
add_opengl_filter.py

Regroupe automatiquement les fichiers de librairies tierces (GLM, glad/OpenGL...)
dans des filtres Visual Studio (Solution Explorer) SANS deplacer les fichiers
sur le disque.
Idempotent : peut etre relance autant de fois que tu veux (apres chaque
regeneration du .vcxproj, dans ton .bat existant par exemple).

IMPORTANT : ferme le fichier .vcxproj.filters dans Visual Studio (ou
recharge la solution) avant de lancer le script, sinon VS peut ecraser
les changements en resauvegardant sa propre version en memoire.

Usage :
    python add_opengl_filter.py [chemin\\vers\\le.vcxproj.filters]
"""

import xml.etree.ElementTree as ET
import uuid
import sys
from pathlib import Path

# ----------------------- CONFIG A ADAPTER -----------------------

# Chemin vers le fichier .vcxproj.filters (ou passe-le en argument du script)
FILTERS_PATH = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("2DPendentLib/2DPendentLib.vcxproj.filters")

# Une entree par librairie a ranger : filtre cible -> mots-cles de detection.
# Detection en 2 passes par mot-cle :
#   1) segment de DOSSIER exact dans le chemin disque (ex: "glm" -> aideur\glm\detail\_fixes.hpp)
#   2) sous-chaine dans le NOM DE FICHIER (ex: "glad" -> glad.c / glad.h directement dans aideur)
# La passe 1 est robuste pour les grosses libs vendorisees dans leur propre dossier (GLM).
# La passe 2 couvre les fichiers isoles sans sous-dossier (glad.c, khrplatform.h...).
LIBRARIES = {
    "Libs\\glad": ["glad"],
    "Libs\\glm": ["glm"],
    "Libs\\KHR": ["khr"],
}

# -----------------------------------------------------------------

NS = "http://schemas.microsoft.com/developer/msbuild/2003"
ET.register_namespace("", NS)


def q(tag: str) -> str:
    return f"{{{NS}}}{tag}"


def detect_target_filter(include_path: str):
    p = include_path.lower().replace("/", "\\")
    parts = p.split("\\")
    filename = parts[-1]
    for target_filter, keywords in LIBRARIES.items():
        for kw in keywords:
            if kw in parts or kw in filename:
                return target_filter
    return None


def ensure_filter_exists(root: ET.Element, filter_group: ET.Element, filter_path: str) -> None:
    """Cree l'entree <Filter Include="..."> si elle n'existe pas deja (et ses parents)."""
    existing = {f.get("Include") for f in root.iter(q("Filter"))}
    parts = filter_path.split("\\")
    for i in range(1, len(parts) + 1):
        sub_path = "\\".join(parts[:i])
        if sub_path in existing:
            continue
        filter_el = ET.SubElement(filter_group, q("Filter"))
        filter_el.set("Include", sub_path)
        guid_el = ET.SubElement(filter_el, q("UniqueIdentifier"))
        guid_el.text = "{" + str(uuid.uuid4()) + "}"
        existing.add(sub_path)


def main():
    if not FILTERS_PATH.exists():
        print(f"Fichier introuvable : {FILTERS_PATH}")
        sys.exit(1)

    tree = ET.parse(FILTERS_PATH)
    root = tree.getroot()

    filter_group = None
    for ig in root.findall(q("ItemGroup")):
        if ig.find(q("Filter")) is not None:
            filter_group = ig
            break
    if filter_group is None:
        filter_group = ET.SubElement(root, q("ItemGroup"))

    for target_filter in LIBRARIES:
        ensure_filter_exists(root, filter_group, target_filter)

    moved = 0
    for ig in root.findall(q("ItemGroup")):
        for tag in ("ClInclude", "ClCompile", "None"):
            for item in ig.findall(q(tag)):
                include = item.get("Include", "")
                target_filter = detect_target_filter(include)
                if target_filter is None:
                    continue
                filt = item.find(q("Filter"))
                if filt is None:
                    filt = ET.SubElement(item, q("Filter"))
                if filt.text != target_filter:
                    filt.text = target_filter
                    moved += 1

    if hasattr(ET, "indent"):
        ET.indent(tree, space="  ")

    tree.write(FILTERS_PATH, xml_declaration=True, encoding="utf-8", method="xml")
    print(f"{moved} fichier(s) range(s) par librairie.")


if __name__ == "__main__":
    main()