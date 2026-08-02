#pragma once
#include "DataStorage.h"

template<typename T>
inline void DataStorage::AddDataToUnorderedMap(const size_t& p_id, const T& data)
{
	auto range = dataMap.equal_range(p_id);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second.type() != typeid(T))
		{
			std::cerr << "Error: Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
			return;
		}

		if (const T* storedData = std::any_cast<T>(&it->second))
		{
			if (*storedData == data)
			{
				std::cerr << "Error: Les données existent déjà pour l'identifiant haché fourni. size_t: " << p_id << std::endl;
				return;
			}
		}
	}
	dataMap.insert({ p_id, data });
}

template<typename T>
inline bool DataStorage::IsDataExist(const size_t& p_id, const T& data)
{
	auto range = dataMap.equal_range(p_id);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (const T* storedData = std::any_cast<T>(&it->second))
		{
			if (*storedData == data)
			{
				return true;
			}
		}
	}
	return false;
}

template<typename T>
inline void DataStorage::CreateDataByStrId(const std::string& p_id, const T& data)
{
	if (p_id.empty() || data == T())
	{
		std::cerr << "Error: L'identifiant ou les données sont vides. p_id: " << p_id << std::endl;
		return;
	}
	size_t hashId = ConvertIdByHaching(p_id);

	AddDataToUnorderedMap(hashId, data);
}

template<typename T>
inline void DataStorage::CreateDataByHashId(const size_t& p_id, const T& data)
{
	if (p_id == 0 || data == T())
	{
		std::cerr << "Error: L'identifiant ou les données sont vides. p_id: " << p_id << std::endl;
		return;
	}


}