#ifndef PCH_H
#define PCH_H

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// Pour les flux d'entr�e/sortie (std::cout, std::cin, etc.)
#include <iostream>
// Pour la classe std::vector, qui est une structure de donn�es de conteneur dynamique utilis�e pour stocker des �l�ments de mani�re s�quentielle.
#include <vector>
// Pour la classe std::map, qui est une structure de donn�es de conteneur associatif utilis�e pour stocker des paires cl�-valeur, o� chaque cl� est unique et associ�e � une valeur.
#include <map>
// Pour la classe std::string, qui est une classe de cha�ne de caract�res utilis�e pour manipuler des cha�nes de texte de mani�re plus facile et efficace que les tableaux de caract�res traditionnels.
#include <string>
// Pour les flux de fichiers (std::ifstream, std::ofstream, etc.), qui sont utilis�s pour lire et �crire des donn�es � partir de fichiers.
#include <fstream>
// Pour les classes std::thread et std::mutex, qui sont utilis�es pour la programmation multithread, permettant d'ex�cuter plusieurs threads d'ex�cution simultan�ment et de synchroniser l'acc�s aux ressources partag�es.
#include <thread>
// Pour la classe std::function, qui est une classe de fonction g�n�rique utilis�e pour stocker et appeler des fonctions, des lambdas, ou des objets fonctionnels de mani�re flexible.
#include <mutex>
// Pour les fonctions math�matiques (std::sqrt, std::sin, etc.) et les constantes math�matiques (std::numbers::pi, etc.).
#include <functional>
// Pour les fonctions de manipulation de cha�nes de caract�res (std::to_string, std::stoi, etc.).
#include <cmath>
// Pour les constantes math�matiques (std::numbers::pi, etc.).
#include <numbers>
// Pour les fonctions de manipulation de conteneurs (std::sort, std::find, etc.).
#include <algorithm>
// Pour les classes std::vector, std::array, etc. utilis�es pour stocker des collections d'�l�ments.
#include <initializer_list>
// Pour les classes std::array, qui est une structure de donn�es de conteneur statique utilis�e pour stocker des �l�ments de mani�re s�quentielle avec une taille fixe.
#include <cassert>
// Pour les classes std::utility, qui contient des fonctions utilitaires telles que std::swap, std::move, etc.
#include <filesystem>

#include <sstream>

#endif