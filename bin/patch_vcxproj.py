"""
patch_vcxproj.py

Force CompileAs=CompileAsC et PrecompiledHeader=NotUsing pour tous les
fichiers .c references dans un .vcxproj genere par SolutionGenerator.exe.

A executer juste apres un `-make`, car le SolutionGenerator ecrase
les overrides par-fichier a chaque regeneration.

Usage:
    python patch_vcxproj.py "chemin\\vers\\projet.vcxproj"
"""

import sys
import xml.etree.ElementTree as ET

NS = "http://schemas.microsoft.com/developer/msbuild/2003"
ET.register_namespace("", NS)


def qualified(tag: str) -> str:
    return f"{{{NS}}}{tag}"


def is_c_file(include_path: str) -> bool:
    return include_path.lower().endswith(".c")


def patch_clcompile_node(node: ET.Element) -> bool:
    changed = False

    compile_as = node.find(qualified("CompileAs"))
    if compile_as is None:
        compile_as = ET.SubElement(node, qualified("CompileAs"))
        changed = True
    if compile_as.text != "CompileAsC":
        compile_as.text = "CompileAsC"
        changed = True

    pch = node.find(qualified("PrecompiledHeader"))
    if pch is None:
        pch = ET.SubElement(node, qualified("PrecompiledHeader"))
        changed = True
    if pch.text != "NotUsing":
        pch.text = "NotUsing"
        changed = True

    return changed


def patch_file(vcxproj_path: str) -> None:
    tree = ET.parse(vcxproj_path)
    root = tree.getroot()

    total_c_files = 0
    total_changed = 0

    for item_group in root.findall(qualified("ItemGroup")):
        for cl_compile in item_group.findall(qualified("ClCompile")):
            include = cl_compile.get("Include", "")
            if not is_c_file(include):
                continue

            total_c_files += 1
            if patch_clcompile_node(cl_compile):
                total_changed += 1
                print(f"  [patched] {include}")
            else:
                print(f"  [ok]      {include}")

    if total_c_files == 0:
        print("  Aucun fichier .c trouve dans ce vcxproj.")
    else:
        print(f"  -> {total_c_files} fichier(s) .c trouve(s), "
              f"{total_changed} modifie(s).")

    tree.write(vcxproj_path, encoding="utf-8", xml_declaration=True)


def main() -> int:
    if len(sys.argv) < 2:
        print("Usage: python patch_vcxproj.py <chemin_vers.vcxproj> [autre.vcxproj ...]")
        return 1

    for path in sys.argv[1:]:
        print(f"Patch de {path} ...")
        try:
            patch_file(path)
        except FileNotFoundError:
            print(f"  ERREUR: fichier introuvable: {path}")
            return 1
        except ET.ParseError as e:
            print(f"  ERREUR: XML invalide dans {path}: {e}")
            return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())