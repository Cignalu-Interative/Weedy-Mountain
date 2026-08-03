#pragma once
#include "DataStorage.h"

template<typename T>
inline void DataStorage::AddDataToUnorderedMap(const size_t& p_id, const T& data)
{
	auto range = m_dataMap.equal_range(p_id);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second.type() != typeid(T))
		{
			std::cerr << "Error: Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
			return;
		}

		const T* storedData = std::any_cast<T>(&it->second);
		if (*storedData == data)
		{
			std::cerr << "Error: Les données existent déjà pour l'identifiant haché fourni. size_t: " << p_id << std::endl;
			return;
		}
	}
	m_dataMap.insert({ p_id, data });
}

template<typename T>
inline bool DataStorage::IsDataExist(const size_t& p_id, const T& data)
{
	auto range = m_dataMap.equal_range(p_id);

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
inline void DataStorage::CreateData(const size_t& p_id, const T& data)
{
	if (p_id == 0)
	{
		std::cerr << "Error: L'identifiant est vide. p_id: " << p_id << std::endl;
		return;
	}

	AddDataToUnorderedMap(p_id, data);
}

template <typename T>
const T* DataStorage::GetData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (const T* storedData = std::any_cast<T>(&it->second))
		{
			return storedData;
		}
	}
	return nullptr;
}

template <typename T>
const T* GetData(const size_t& p_id, size_t p_index)
{
	if (p_index >= m_dataMap.count(p_id))
	{
		std::cerr << "Error: Index out of range. p_index: " << p_index << ", count: " << m_dataMap.count(p_id) << std::endl;
		return nullptr;
	}
	auto range = m_dataMap.equal_range(p_id);

	auto it = range.first;
	std::advance(it, p_index);

	const T* storedData = std::any_cast<T>(&it->second);

	return storedData;
}

template <typename T>
std::vector<const T*> DataStorage::GetAllData(const size_t& p_id)
{
	std::vector<const T*> dataList;
	size_t count = m_dataMap.count(p_id);
	dataList.reserve(count);

	auto range = m_dataMap.equal_range(p_id);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (const T* storedData = std::any_cast<T>(&it->second))
		{
			dataList.push_back(storedData);
		}
		else
		{
			std::cerr << "Error: Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
		}
	}
	return dataList;
}