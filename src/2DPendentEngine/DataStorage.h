#pragma once
#include <string>
#include <unordered_map>
#include <any>

// Un concept est une condition qui doit être remplie pour qu'un type soit considéré comme valide pour un certain usage.
// Concept : vérifie que T supporte les opérateurs += et -=
// pour les fonctions add et decrease

template <typename T>
concept CONSEPT_Set = requires(T a, T b)
{
	a = b;
};
template <typename T>
concept CONSEPT_Add_AND_Decrease = requires(T a, T b)
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
	bool AddDataToUnorderedMap(const size_t& p_id, const T& data);

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
	bool CreateData(const size_t& p_id, const T& data);

	// Récupère la première donnée associée à l'identifiant haché fourni.
	const std::any* GetFirstData(const size_t& p_id);
	// Récupère la dernière donnée associée à l'identifiant haché fourni.
	const std::any* GetLastData(const size_t& p_id);
	// Récupère la donnée associée à l'identifiant haché fourni et à l'index fourni.
	const std::any* GetDataAtIndex(const size_t& p_id, size_t p_index = 0);
	// Récupère toutes les données associées à l'identifiant haché fourni.
	std::vector<const std::any*> GetAllDataById(const size_t& p_id);

	// Récupère la première donnée associée à l'identifiant haché fourni, avec le type de données spécifié.
	template <typename T>
	const T* GetFirstData(const size_t& p_id);
	// Récupère la dernière donnée associée à l'identifiant haché fourni, avec le type de données spécifié.
	template <typename T>
	const T* GetLastData(const size_t& p_id);
	// Récupère la donnée associée à l'identifiant haché fourni et à l'index fourni, avec le type de données spécifié.
	template <typename T>
	const T* GetDataAtIndex(const size_t& p_id, size_t p_index = 0);
	// Récupère toutes les données associées à l'identifiant haché fourni, avec le type de données spécifié.
	template <typename T>
	std::vector<const T*> GetAllDataById(const size_t& p_id);

	// Retourne le nombre de données associées à l'identifiant haché fourni.
	size_t GetDataCount(const size_t& p_id);
	// Retourne l'index de la donnée associée à l'identifiant haché fourni et à la donnée fournie, avec le type et valeur spécifiés.
	template <typename T>
	size_t GetIndexOfData(const size_t& p_id, const T& data);

	// Clear = supprime toutes les données associées à l'identifiant haché.
	bool ClearDataById(const size_t& p_id);
	// Delete = supprime la première donnée associée à l'identifiant haché.
	bool DeleteFirstData(const size_t& p_id);
	// Delete = supprime la dernière donnée associée à l'identifiant haché.
	bool DeleteLastData(const size_t& p_id);
	// Delete = supprime une seule donnée associée à l'identifiant haché et à l'index fourni.
	bool DeleteDataAtIndex(const size_t& p_id, size_t p_index = 0);

	// Clear = supprime toutes les données associées à l'identifiant haché, avec le type de données spécifié.
	template <typename T>
	bool ClearDataById(const size_t& p_id);
	// Delete = supprime la première donnée associée à l'identifiant haché, avec le type de données spécifié.
	template <typename T>
	bool DeleteFirstData(const size_t& p_id);
	// Delete = supprime la dernière donnée associée à l'identifiant haché, avec le type de données spécifié.
	template <typename T>
	bool DeleteLastData(const size_t& p_id);
	// Delete = supprime une seule donnée associée à l'identifiant haché et à l'index fourni, avec le type de données spécifié.
	template <typename T>
	bool DeleteDataAtIndex(const size_t& p_id, size_t p_index = 0);

	// Set = met à jour toutes les données associées à l'identifiant haché.
	template <typename T>
	bool SetDataById(const size_t& p_id, const T& data, size_t p_index = 0);
	// Add = ajoute une seule donnée associée à l'identifiant haché.
	// Decrease = diminue une seule donnée associée à l'identifiant haché.

};

#include "DataStorage.inl"
