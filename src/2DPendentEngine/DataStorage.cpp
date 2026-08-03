#include "pch.h"
#include "DataStorage.h"


DataStorage::DataStorage()
{
	m_dataMap.reserve(100); // Réserve de l'espace pour 100 éléments pour éviter les réallocations fréquentes
}

DataStorage::~DataStorage()
{

}

size_t DataStorage::ConvertIdByHaching(const std::string& p_id)
{
	size_t hashId = std::hash<std::string>{} (p_id);

	return hashId;
}

bool DataStorage::IsIdExist(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);
	for (auto it = range.first; it != range.second; ++it)
	{
		return true;
	}
	return false;
}

bool DataStorage::IsIdExist(const std::string& p_id)
{
	size_t hashId = std::hash<std::string>{}(p_id);

	auto range = m_dataMap.equal_range(hashId);

	for (auto it = range.first; it != range.second; ++it)
	{
		return true;
	}
	return false;
}

const std::any* DataStorage::GetFirstData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		return nullptr; // Aucun élément trouvé pour l'identifiant donné
	}

	return &range.first->second;
}

const std::any* DataStorage::GetLastData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);
	
	if (range.first == range.second)
	{
		return nullptr; // Aucun élément trouvé pour l'identifiant donné
	}

	auto it = range.second;
	--it;
	return &it->second;
}

const std::any* DataStorage::GetDataAtIndex(const size_t& p_id, size_t p_index)
{
	if (p_index >= m_dataMap.count(p_id))
	{
		std::cerr << "Error: Index out of range. p_index: " << p_index << ", count: " << m_dataMap.count(p_id) << std::endl;
		return nullptr;
	}
	auto range = m_dataMap.equal_range(p_id);

	auto it = range.first;
	std::advance(it, p_index);
	return &it->second;
}

std::vector<const std::any*> DataStorage::GetAllDataById(const size_t& p_id)
{
	std::vector<const std::any*> dataList;
	std::any typeCheck;
	size_t count = m_dataMap.count(p_id);
	dataList.reserve(count);

	auto range = m_dataMap.equal_range(p_id);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (dataList.empty())
		{
			typeCheck = &it->second.type();
		}
		if (typeCheck.type() != it->second.type())
		{
			std::cerr << "Error: les valeur ne sont pas du même type pour l'ID: " << p_id << std::endl;
			continue;
		}
		dataList.push_back(&it->second);
	}
	return dataList;
}

size_t DataStorage::GetDataCount(const size_t& p_id)
{
	return m_dataMap.count(p_id);
}

bool DataStorage::ClearDataById(const size_t& p_id)
{
	size_t count = m_dataMap.count(p_id);
	if (count == 0)
	{
		std::cerr << "Error: pas de données trouvées pour l'ID: " << p_id << std::endl;
		return false;
	}

	size_t erasedCount = m_dataMap.erase(p_id);

	if (erasedCount != count)
	{
		std::cerr << "Error: le nombre de données supprimées (" << erasedCount << ") ne correspond pas au nombre attendu (" << count << ") pour l'ID: " << p_id << std::endl;
		return false;
	}
	return true;
}

bool DataStorage::DeleteFirstData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Error: pas de données trouvées pour l'ID: " << p_id << std::endl;
		return false;
	}

	m_dataMap.erase(range.first);
	return true;
}

bool DataStorage::DeleteLastData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Error: pas de données trouvées pour l'ID: " << p_id << std::endl;
		return false;
	}

	m_dataMap.erase(--range.second);
	return true;
}

bool DataStorage::DeleteDataAtIndex(const size_t& p_id, size_t p_index)
{
	if (p_index >= m_dataMap.count(p_id))
	{
		std::cerr << "Error: Index out of range. p_index: " << p_index << ", count: " << m_dataMap.count(p_id) << std::endl;
		return false;
	}

	auto range = m_dataMap.equal_range(p_id);

	if (range.first == range.second)
	{
		std::cerr << "Error: pas de données trouvées pour l'ID: " << p_id << std::endl;
		return false;
	}

	auto it = range.first;
	std::advance(it, p_index);

	m_dataMap.erase(it);
	return true;
}

