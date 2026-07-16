#pragma once
#include <string>

class ItemData
{
private:
	
	std::string m_ID;

	std::string m_path;

	std::string m_name;
	std::string m_description;

	int m_buyPrice = 0;
	int m_sellPrice = 0;

	int m_maxStack = 64;

	std::string m_useActionID;

public:

	ItemData(const std::string& ID, const std::string& path, const std::string& name, const std::string& description, int buyPrice, int sellPrice, int maxStack, const std::string& useActionID = "");

	const std::string& GetID() const;
	const std::string& GetPath() const;
	const std::string& GetName() const;
	const std::string& GetDescription() const;
	int GetBuyPrice() const;
	int GetSellPrice() const;
	int GetMaxStack() const;
	const std::string& GetUseActionID() const;
};