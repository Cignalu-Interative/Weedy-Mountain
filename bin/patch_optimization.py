#!/usr/bin/env python3
"""
patch_optimization.py

Ajoute/corrige les parametres d'optimisation Release manquants sur tous les
.vcxproj trouves recursivement sous un dossier racine (Game, Lib, Engine...).

100% stdlib : aucune dependance externe (pas besoin de pip install lxml).

Comportement :
- Balise Release existante avec une mauvaise valeur -> valeur corrigee,
  la Condition d'origine n'est jamais modifiee.
- Balise Release absente -> creee avec une Condition par defaut.
- Balise Debug -> jamais touchee.

Usage (lance depuis bin/, scanne ../ide par defaut) :
    python patch_optimization.py [dossier_racine] [--apply]

Sans --apply : affiche uniquement les changements proposes (dry-run).
Avec --apply : applique les changements (backup .bak cree automatiquement),
               demande confirmation pour chaque fichier modifie.
"""

import sys
import argparse
from pathlib import Path
import xml.etree.ElementTree as ET

NS_URI = "http://schemas.microsoft.com/developer/msbuild/2003"
ET.register_namespace("", NS_URI)  # evite le prefixe ns0: a l'ecriture

CLCOMPILE_TARGETS = [
    {"name": "Optimization", "value": "MaxSpeed",
     "condition": "'$(Configuration)'=='Release'"},                          # /O2
    {"name": "InlineFunctionExpansion", "value": "AnySuitable",
     "condition": "'$(Configuration)|$(Platform)'=='Release|x64'"},           # /Ob2
    {"name": "IntrinsicFunctions", "value": "true",
     "condition": "'$(Configuration)'=='Release'"},                          # /Oi
    {"name": "FavorSizeOrSpeed", "value": "Speed",
     "condition": "'$(Configuration)|$(Platform)'=='Release|x64'"},           # /Ot
    {"name": "OmitFramePointers", "value": "true",
     "condition": "'$(Configuration)|$(Platform)'=='Release|x64'"},           # /Oy
]

LINK_TARGETS = [
    {"name": "LinkTimeCodeGeneration", "value": "UseLinkTimeCodeGeneration",
     "condition": "'$(Configuration)'=='Release'"},                          # /LTCG
]

# WholeProgramOptimization vit dans PropertyGroup Label="Configuration",
# pas dans ItemDefinitionGroup -> gere a part.
PROPERTYGROUP_TARGETS = [
    {"name": "WholeProgramOptimization", "value": "true",
     "condition": "'$(Configuration)'=='Release'"},                          # /GL
]


def tag(name: str) -> str:
    return f"{{{NS_URI}}}{name}"


def elem_applies_to_release(elem) -> bool:
    """Un element s'applique a Release si sa Condition contient 'Release'."""
    return "Release" in elem.get("Condition", "")


def append_with_indent(parent, new_elem, parent_depth: int):
    """
    Ajoute new_elem comme dernier enfant de parent en reproduisant
    l'indentation Visual Studio (2 espaces par niveau), quelle que soit
    la profondeur ou le nombre d'enfants deja presents.
    """
    child_indent = "\n" + "  " * (parent_depth + 1)
    closing_indent = "\n" + "  " * parent_depth

    children = list(parent)
    if children:
        children[-1].tail = child_indent
    else:
        parent.text = child_indent
    new_elem.tail = closing_indent
    parent.append(new_elem)


def ensure_param(section, target: dict, changes: list, section_label: str, depth: int):
    """
    Garantit que TOUTE balise <target['name']> qui s'applique a Release
    porte bien la valeur target['value'], sans toucher a sa Condition.
    Cree la balise seulement si aucune variante Release n'existe.
    """
    existing_release = [
        e for e in section.findall(tag(target["name"]))
        if elem_applies_to_release(e)
    ]

    if existing_release:
        for e in existing_release:
            if e.text != target["value"]:
                changes.append({
                    "section": section_label, "param": target["name"],
                    "old": e.text, "new": target["value"], "action": "update",
                    "condition": e.get("Condition", ""),
                })
                e.text = target["value"]
        return

    new_elem = ET.Element(tag(target["name"]))
    new_elem.set("Condition", target["condition"])
    new_elem.text = target["value"]
    append_with_indent(section, new_elem, depth)
    changes.append({
        "section": section_label, "param": target["name"],
        "old": None, "new": target["value"], "action": "create",
        "condition": target["condition"],
    })


def find_vcxproj_files(root: Path):
    return sorted(root.rglob("*.vcxproj"))


def process_file(path: Path, apply_changes: bool):
    tree = ET.parse(str(path))
    root = tree.getroot()

    changes = []

    # 1) PropertyGroup Label="Configuration" -> WholeProgramOptimization (depth=1)
    for pg in root.findall(tag("PropertyGroup")):
        if pg.get("Label") == "Configuration":
            for target in PROPERTYGROUP_TARGETS:
                ensure_param(pg, target, changes, "PropertyGroup", depth=1)

    # 2) ItemDefinitionGroup (depth=1) -> ClCompile / Link (depth=2)
    for item_def_group in root.findall(tag("ItemDefinitionGroup")):
        clcompile = item_def_group.find(tag("ClCompile"))
        if clcompile is not None:
            for target in CLCOMPILE_TARGETS:
                ensure_param(clcompile, target, changes, "ClCompile", depth=2)

        link = item_def_group.find(tag("Link"))
        if link is not None:
            for target in LINK_TARGETS:
                ensure_param(link, target, changes, "Link", depth=2)

    if not changes:
        print(f"  -> OK, deja conforme : {path.name}")
        return

    print(f"\n=== {path} ===")
    for c in changes:
        old_disp = c["old"] if c["old"] is not None else "(absent)"
        cond_disp = c.get("condition", "")
        print(f"  [{c['section']}] {c['param']}: {old_disp} -> {c['new']}  ({c['action']}, Condition=\"{cond_disp}\")")

    if not apply_changes:
        print("  (dry-run, rien applique)")
        return

    confirm = input(f"  Appliquer ces {len(changes)} changement(s) a {path.name} ? [o/N] ").strip().lower()
    if confirm != "o":
        print("  -> ignore.")
        return

    backup_path = path.with_suffix(path.suffix + ".bak")
    backup_path.write_bytes(path.read_bytes())
    print(f"  -> backup cree : {backup_path}")

    tree.write(str(path), xml_declaration=True, encoding="utf-8")
    print(f"  -> fichier mis a jour : {path}")


def main():
    parser = argparse.ArgumentParser(description="Ajoute/corrige les parametres d'optimisation Release manquants sur les .vcxproj")
    parser.add_argument("root", type=str, nargs="?", default="../ide",
                         help="Dossier racine a scanner recursivement (par defaut : ../ide, lance depuis bin/)")
    parser.add_argument("--apply", action="store_true", help="Applique les changements (sinon dry-run)")
    args = parser.parse_args()

    root_path = Path(args.root)
    if not root_path.exists():
        print(f"Erreur : dossier introuvable -> {root_path}")
        sys.exit(1)

    files = find_vcxproj_files(root_path)
    if not files:
        print(f"Aucun .vcxproj trouve sous {root_path}")
        sys.exit(0)

    print(f"{len(files)} fichier(s) .vcxproj trouve(s) :")
    for f in files:
        print(f"  - {f}")

    for f in files:
        process_file(f, apply_changes=args.apply)


if __name__ == "__main__":
    main()
