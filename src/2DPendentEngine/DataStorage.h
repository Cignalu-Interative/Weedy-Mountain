#pragma once
#include <string>
#include <unordered_map>

template <typename T>
class DataStorage
{
private:

	std::unordered_multimap<size_t, T> dataMap;
	std::unordered_multimap<size_t, std::string> hashIdMap;

	DataStorage();
	~DataStorage();

	// Ajoute des données à la map non ordonnée en utilisant l'identifiant haché fourni.
	void AddDataToUnorderedMap(const size_t& p_id, const T& data);

public:

	// Crée un identifiant haché à partir de la chaîne de caractères fournie.
	size_t CreateIdByHaching(const std::string& p_id);

	// Renvoie la chaîne de caractères associée à l'identifiant haché fourni.
	// Si l'identifiant n'existe pas, renvoie une chaîne vide.
	const std::string& GetStrId(const size_t& p_id);
	// Renvoie l'identifiant haché associé à la chaîne de caractères fournie.
	// Si la chaîne n'existe pas, renvoie 0.
	const size_t& GetHashId(const std::string& p_id);

	// Crée une entrée de données associée à l'identifiant de chaîne fourni.
	// [PERFORMANCE] = plus lent mais plus lisible.
	void CreateDataByStrId(const std::string& p_id, const T& data);
	// Crée une entrée de données associée à l'identifiant haché fourni.
	// [PERFORMANCE] = plus rapide mais moins lisible.
	void CreateDataByHashId(const size_t& p_id, const T& data);



};

#include "DataStorage.inl"
