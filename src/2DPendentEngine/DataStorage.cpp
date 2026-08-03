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

const std::any* DataStorage::GetData(const size_t& p_id)
{
	auto range = m_dataMap.equal_range(p_id);

	for (auto it = range.first; it != range.second; ++it)
	{
		return &it->second;	
	}
	return nullptr;
}

const std::any* DataStorage::GetData(const size_t& p_id, size_t p_index)
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

std::vector<const std::any*> DataStorage::GetAllData(const size_t& p_id)
{
	std::vector<const std::any*> dataList;
	size_t count = m_dataMap.count(p_id);
	dataList.reserve(count);

	auto range = m_dataMap.equal_range(p_id);
	for (auto it = range.first; it != range.second; ++it)
	{
		dataList.push_back(&it->second);
	}
	return dataList;
}

size_t DataStorage::GetDataCount(const size_t& p_id)
{
	return m_dataMap.count(p_id);
}

void DataStorage::Clear(const size_t& p_id)
{
	m_dataMap.erase(p_id);
}
