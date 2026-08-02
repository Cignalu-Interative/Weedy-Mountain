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
		if (it->first == p_id)
		{
			return true;
		}
	}
	return false;
}

bool DataStorage::IsIdExist(const std::string& p_id)
{
	size_t hashId = std::hash<std::string>{}(p_id);

	auto range = m_dataMap.equal_range(hashId);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->first == hashId)
		{
			return true;
		}
	}
	return false;
}

std::vector<std::any> DataStorage::GetAllData(const size_t& p_id)
{
	return std::vector<std::any>();
}
