#pragma once

#include "ItemData.h"
#include <unordered_map>
#include <functional>

class ItemDataBase
{
private:

	std::unordered_map<std::string, ItemData> m_items;
	ItemDataBase() = default;

	std::unordered_map<int, std::string> m_internalIdToStringId;

public:

	static ItemDataBase* Get();

	bool LoadFromJSON(const std::string& filePath, std::function<bool(const std::string&)> validateActionID = nullptr);

	ItemData* GetItem(const std::string& ID);

	const std::unordered_map<std::string, ItemData>& GetAll() const;

	const std::string& ResolveInternalID(int internalID) const;
};