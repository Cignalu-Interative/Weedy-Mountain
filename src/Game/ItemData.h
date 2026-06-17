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

public:

	ItemData(const std::string& ID, const std::string& path, const std::string& name, const std::string& description, int buyPrice, int sellPrice, int maxStack);

	std::string& GetID();
	std::string& GetPath();
	std::string& GetName();
	std::string& GetDescription();
	int GetBuyPrice();
	int GetSellPrice();
	int GetMaxStack();
};