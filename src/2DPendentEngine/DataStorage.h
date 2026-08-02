#pragma once
#include <string>
#include <unordered_map>
#include <any>


class DataStorage
{
private:

	// Map non ordonnée pour stocker les données associées à des identifiants hachés.
	// avec std::any pour permettre de stocker des types de données différents.
	// mais un id ne peut contenir qu'un seul type de données, si on essaye d'ajouter un autre type de données pour le même id, une erreur sera affichée.
	std::unordered_multimap<size_t, std::any> dataMap;
	// Map non ordonnée pour stocker les chaînes de caractères associées à des identifiants hachés.
	std::unordered_multimap<size_t, std::string> hashIdMap;

	// Constructeur de la classe DataStorage.
	DataStorage();
	// Destructeur de la classe DataStorage.
	~DataStorage();

	// Ajoute des données à la map non ordonnée en utilisant l'identifiant haché fourni.
	template <typename T>
	void AddDataToUnorderedMap(const size_t& p_id, const T& data);

public:

	// Crée un identifiant haché à partir de la chaîne de caractères fournie.
	size_t ConvertIdByHaching(const std::string& p_id);

	// Vérifie si l'identifiant haché fourni existe dans la unordered_multimap.
	bool IsIdExist(const size_t& p_id);
	// Vérifie si la chaîne de caractères fournie existe dans la unordered_multimap.
	bool IsIdExist(const std::string& p_id);

	// Vérifie si les données associées à l'identifiant haché fourni existent dans la unordered_multimap.
	template <typename T>
	bool IsDataExist(const size_t& p_id, const T& data);

	// Renvoie la chaîne de caractères associée à l'identifiant haché fourni.
	// Si l'identifiant n'existe pas dans la unordered_multimap, renvoie une chaîne vide.
	const std::string& GetStrId(const size_t& p_id);
	// Renvoie l'identifiant haché associé à la chaîne de caractères fournie.
	// Si la chaîne n'existe pas dans la unordered_multimap, renvoie 0.
	const size_t& GetHashId(const std::string& p_id);

	// Crée une entrée de données associée à l'identifiant de chaîne fourni.
	// [PERFORMANCE] = plus lent mais plus lisible.
	// [TYPE] = id peut contenir n'importe quel type de données, mais un id ne peut contenir qu'un seul type de données.
	template <typename T>
	void CreateDataByStrId(const std::string& p_id, const T& data);
	// Crée une entrée de données associée à l'identifiant haché fourni.
	// [PERFORMANCE] = plus rapide mais moins lisible.
	// [TYPE] = id peut contenir n'importe quel type de données, mais un id ne peut contenir qu'un seul type de données.
	template <typename T>
	void CreateDataByHashId(const size_t& p_id, const T& data);



};

#include "DataStorage.inl"
