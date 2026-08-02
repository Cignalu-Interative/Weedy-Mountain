#include "pch.h"
#include "DataStorage.h"


DataStorage::DataStorage()
{


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
	auto range = hashIdMap.equal_range(p_id);
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

	auto range = hashIdMap.equal_range(hashId);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->first == hashId)
		{
			return true;
		}
	}
	return false;
}

const std::string& DataStorage::GetStrId(const size_t& p_id)
{
	static const std::string emptyString = "";

	auto range = hashIdMap.equal_range(p_id);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->first == p_id)
		{
			return it->second;
		}
	}
	return emptyString;
}

const size_t& DataStorage::GetHashId(const std::string& p_id)
{
	static const size_t emptyId = 0;
	size_t hashId = std::hash<std::string>{}(p_id);

	auto range = hashIdMap.equal_range(hashId);

	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->first == hashId)
		{
			return hashId;
		}
	}
	return emptyId;
}
