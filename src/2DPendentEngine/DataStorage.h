#pragma once
#include <string>
#include <unordered_map>
#include <any>

// Un concept est une condition qui doit être remplie pour qu'un type soit considéré comme valide pour un certain usage.
// Concept : vérifie que T supporte les opérateurs += et -=
// pour les fonctions add et decrease
template <typename T>
concept Add_AND_Decrease = requires(T a, T b)
{
	a += b;
	a -= b;
};

class DataStorage
{
private:

	// Map non ordonnée pour stocker les données associées à des identifiants hachés.
	// avec std::any pour permettre de stocker des types de données différents.
	// mais un id ne peut contenir qu'un seul type de données, si on essaye d'ajouter un autre type de données pour le même id, une erreur sera affichée.
	std::unordered_multimap<size_t, std::any> m_dataMap;

	// Ajoute des données à la map non ordonnée en utilisant l'identifiant haché fourni.
	template <typename T>
	void AddDataToUnorderedMap(const size_t& p_id, const T& data);

public:

	// Constructeur de la classe DataStorage.
	DataStorage();
	// Destructeur de la classe DataStorage.
	~DataStorage();

	// Crée un identifiant haché à partir de la chaîne de caractères fournie.
	size_t ConvertIdByHaching(const std::string& p_id);

	// Vérifie si l'identifiant haché fourni existe dans la unordered_multimap.
	bool IsIdExist(const size_t& p_id);
	// Vérifie si la chaîne de caractères fournie existe dans la unordered_multimap.
	bool IsIdExist(const std::string& p_id);

	// Vérifie si les données associées à l'identifiant haché fourni existent dans la unordered_multimap.
	template <typename T>
	bool IsDataExist(const size_t& p_id, const T& data);

	// Crée une entrée de données associée à l'identifiant haché fourni.
	// [PERFORMANCE] = plus rapide mais moins lisible.
	// [TYPE] = id peut contenir n'importe quel type de données, mais un id ne peut contenir qu'un seul type de données.
	template <typename T>
	void CreateData(const size_t& p_id, const T& data);

	template <typename T>
	T* GetData(const size_t& p_id);

	// TODO: Ajouter une fonction pour récupérer toutes les données associées à un identifiant haché.
	std::vector<std::any> GetAllData(const size_t& p_id);

	// TODO: Ajouter une fonction pour supprimer des données associées à un identifiant haché.
	// Clear = supprime toutes les données associées à l'identifiant haché.
	// Delete = supprime une seule donnée associée à l'identifiant haché.

	// TODO: Ajouter une fonction pour mettre à jour les données associées à un identifiant haché.
	// Set = met à jour toutes les données associées à l'identifiant haché.
	// Add = ajoute une seule donnée associée à l'identifiant haché.
	// Decrease = diminue une seule donnée associée à l'identifiant haché.

};

#include "DataStorage.inl"
