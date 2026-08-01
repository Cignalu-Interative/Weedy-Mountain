#pragma once
#include "DataStorage.h"

template<typename T>
inline DataStorage<T>::DataStorage()
{


}

template<typename T>
inline DataStorage<T>::~DataStorage()
{


}

template<typename T>
inline void DataStorage<T>::AddDataToUnorderedMap(const size_t& p_id, const T& data)
{
	/*if (auto it = hashIdMap.find(p_id); it == hashIdMap.end())
	{
		std::cerr << "Error: L'identifiant n'existe pas dans le hashIdMap. size_t: " << p_id << std::endl;
		return;
	}

	dataMap[p_id] = data;*/
}

template<typename T>
inline size_t DataStorage<T>::CreateIdByHaching(const std::string& p_id)
{
	size_t hashId = std::hash<std::string>{} (p_id);

	return hashId;
}

template<typename T>
inline const std::string& DataStorage<T>::GetStrId(const size_t& p_id)
{
	/*static const std::string emptyString = "";

	if (auto it = hashIdMap.find(p_id); it != hashIdMap.end())
	{
		return it->second;
	}
	return emptyString;*/
}

template<typename T>
inline const size_t& DataStorage<T>::GetHashId(const std::string& p_id)
{
	/*static const size_t emptyId = 0;
	size_t hashId = std::hash<std::string>{}(p_id);

	if (auto it = hashIdMap.find(hashId); it != hashIdMap.end())
	{
		return hashId;
	}
	return emptyId;*/
}

template<typename T>
inline void DataStorage<T>::CreateDataByStrId(const std::string& p_id, const T& data)
{
	if (p_id.empty() || data == T())
	{
		std::cerr << "Error: L'identifiant ou les données sont vides. p_id: " << p_id << std::endl;
		return;
	}
	size_t hashId = GetHashId(p_id);

	if (hashId == 0)
	{
		std::cerr << "Error: L'identifiant n'existe pas. p_id: " << p_id << std::endl;
		return;
	}



}

template<typename T>
inline void DataStorage<T>::CreateDataByHashId(const size_t& p_id, const T& data)
{



}
