#ifndef PCH_H
#define PCH_H

// Pour les flux d'entrée/sortie (std::cout, std::cin, etc.)
#include <iostream>
// Pour la classe std::vector, qui est une structure de données de conteneur dynamique utilisée pour stocker des éléments de manière séquentielle.
#include <vector>
// Pour la classe std::map, qui est une structure de données de conteneur associatif utilisée pour stocker des paires clé-valeur, où chaque clé est unique et associée à une valeur.
#include <map>
// Pour la classe std::string, qui est une classe de chaîne de caractères utilisée pour manipuler des chaînes de texte de manière plus facile et efficace que les tableaux de caractères traditionnels.
#include <string>
// Pour les flux de fichiers (std::ifstream, std::ofstream, etc.), qui sont utilisés pour lire et écrire des données à partir de fichiers.
#include <fstream>
// Pour les classes std::thread et std::mutex, qui sont utilisées pour la programmation multithread, permettant d'exécuter plusieurs threads d'exécution simultanément et de synchroniser l'accès aux ressources partagées.
#include <thread>
// Pour la classe std::function, qui est une classe de fonction générique utilisée pour stocker et appeler des fonctions, des lambdas, ou des objets fonctionnels de manière flexible.
#include <mutex>
// Pour les fonctions mathématiques (std::sqrt, std::sin, etc.) et les constantes mathématiques (std::numbers::pi, etc.).
#include <functional>
// Pour les fonctions de manipulation de chaînes de caractères (std::to_string, std::stoi, etc.).
#include <cmath>
// Pour les constantes mathématiques (std::numbers::pi, etc.).
#include <numbers>
// Pour les fonctions de manipulation de conteneurs (std::sort, std::find, etc.).
#include <algorithm>
// Pour les classes std::vector, std::array, etc. utilisées pour stocker des collections d'éléments.
#include <initializer_list>
// Pour les classes std::array, qui est une structure de données de conteneur statique utilisée pour stocker des éléments de manière séquentielle avec une taille fixe.
#include <cassert>
// Pour les classes std::utility, qui contient des fonctions utilitaires telles que std::swap, std::move, etc.
#include <filesystem>

#endif