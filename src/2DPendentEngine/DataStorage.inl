#pragma once
#include "DataStorage.h"

template<typename T>
inline bool DataStorage::AddDataToUnorderedMap(const size_t& p_id, const T& data)
{
	auto range = m_dataMap.equal_range(p_id);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second.type() != typeid(T))
		{
			std::cerr << "Engine/DataStorage.inl/AddDataToUnorderedMap/ Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
			return false;
		}

		const T* storedData = std::any_cast<T>(&it->second);
		if (*storedData == data)
		{
			std::cerr << "Engine/DataStorage.inl/AddDataToUnorderedMap/ Les données existent déjà pour l'identifiant haché fourni. size_t: " << p_id << std::endl;
			return false;
		}
	}
	m_dataMap.insert({ p_id, data });
	return true;
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
	std::cerr << "Engine/DataStorage.inl/IsDataExist/ Les données n'existent pas pour l'identifiant haché fourni. size_t: " << p_id << std::endl;
	return false;
}

template<typename T>
inline bool DataStorage::CreateData(const size_t& p_id, const T& data)
{
	if (p_id == 0)
	{
		std::cerr << "Engine/DataStorage.inl/CreateData/ L'identifiant est vide. p_id: " << p_id << std::endl;
		return false;
	}
	bool success = AddDataToUnorderedMap(p_id, data);
	return success;
}

template <typename T>
const T* DataStorage::GetFirstData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/GetFirstData/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return nullptr;
	}

	const T* storedData = std::any_cast<T>(&range.first->second);
	return storedData;
}

template <typename T>
const T* DataStorage::GetLastData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/GetLastData/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return nullptr;
	}

	const T* storedData = std::any_cast<T>(&(--range.second)->second);
	return storedData;
}

template <typename T>
const T* DataStorage::GetDataAtIndex(const size_t& p_id, size_t p_index)
{
	if (p_index >= m_dataMap.count(p_id))
	{
		std::cerr << "Engine/DataStorage.inl/GetDataAtIndex/ Index hors limites. p_index: " << p_index << ", count: " << m_dataMap.count(p_id) << std::endl;
		return nullptr;
	}
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/GetDataAtIndex/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return nullptr;
	}

	auto it = range.first;
	std::advance(it, p_index);

	const T* storedData = std::any_cast<T>(&it->second);
	return storedData;
}

template <typename T>
std::vector<const T*> DataStorage::GetAllDataById(const size_t& p_id)
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
			std::cerr << "Engine/DataStorage.inl/GetAllDataById/ Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
		}
	}
	return dataList;
}

template<typename T>
inline size_t DataStorage::GetIndexOfData(const size_t& p_id, const T& data)
{
	auto range = m_dataMap.equal_range(p_id);
	size_t index = 0;

	for (auto it = range.first; it != range.second; ++it)
	{
		if (const T* storedData = std::any_cast<T>(&it->second))
		{
			if (*storedData == data)
			{
				return index;
			}
		}
		++index;
	}
	std::cerr << "Engine/DataStorage.inl/GetIndexOfData/ Les données n'ont pas été trouvées pour l'identifiant haché fourni. size_t: " << p_id << std::endl;
	return static_cast<size_t>(-1);; // Retourne -1 si la donnée n'est pas trouvée
}

template <typename T>
bool DataStorage::ClearDataById(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/ClearDataById/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return false;
	}

	bool erasedAny = false;

	for (auto it = range.first; it != range.second; )
	{
		if (it->second.type() == typeid(T))
		{
			it = m_dataMap.erase(it);
			erasedAny = true;
		}
		else
		{
			++it;
		}
	}

	if (!erasedAny)
	{
		std::cerr << "Engine/DataStorage.inl/ClearDataById/ Aucune donnée du type demandé (" << typeid(T).name() << ") trouvée pour l'ID: " << p_id << std::endl;
		return false;
	}
	return erasedAny;
}

template <typename T>
bool DataStorage::DeleteFirstData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/DeleteFirstData/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return false;
	}

	auto it = range.first;
	if (it->second.type() != typeid(T))
	{
		std::cerr << "Engine/DataStorage.inl/DeleteFirstData/ Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
		return false;
	}

	m_dataMap.erase(it);
	return true;
}

template <typename T>
bool DataStorage::DeleteLastData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/DeleteLastData/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return false;
	}

	auto it = --range.second;
	if (it->second.type() != typeid(T))
	{
		std::cerr << "Engine/DataStorage.inl/DeleteLastData/ Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
		return false;
	}

	m_dataMap.erase(it);
	return true;
}

template <typename T>
bool DataStorage::DeleteDataAtIndex(const size_t& p_id, size_t p_index)
{
	if (p_index >= m_dataMap.count(p_id))
	{
		std::cerr << "Engine/DataStorage.inl/DeleteDataAtIndex/ Index hors limites. p_index: " << p_index << ", count: " << m_dataMap.count(p_id) << std::endl;
		return false;
	}

	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Engine/DataStorage.inl/DeleteDataAtIndex/ Aucun élément trouvé pour l'identifiant donné. p_id: " << p_id << std::endl;
		return false;
	}

	auto it = range.first;
	std::advance(it, p_index);

	if (it->second.type() != typeid(T))
	{
		std::cerr << "Engine/DataStorage.inl/DeleteDataAtIndex/ Type incompatible (" << typeid(T).name() << ") pour l'ID " << p_id << " qui contient déjà du " << it->second.type().name() << std::endl;
		return false;
	}

	m_dataMap.erase(it);
	return true;

}

template<typename T>
inline bool DataStorage::SetDataById(const size_t& p_id, const T& data, size_t p_index)
{
	


	return true;
}
