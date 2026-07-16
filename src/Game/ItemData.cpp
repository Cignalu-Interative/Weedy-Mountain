#include "pch.h"
#include "ItemData.h"

ItemData::ItemData(const std::string& ID, const std::string& path, const std::string& name, const std::string& description, int buyPrice, int sellPrice, int maxStack, const std::string& useActionID) 
: m_ID(ID), m_path(path), m_name(name), m_description(description), m_buyPrice(buyPrice), m_sellPrice(sellPrice), m_maxStack(maxStack), m_useActionID(useActionID) {}

const std::string& ItemData::GetID() const
{
	return m_ID;
}

const std::string& ItemData::GetPath() const
{
	return m_path;
}

const std::string& ItemData::GetName() const
{
	return m_name;
}

const std::string& ItemData::GetDescription() const
{
	return m_description;
}

int ItemData::GetBuyPrice() const
{
	return m_buyPrice;
}

int ItemData::GetSellPrice() const
{
	return m_sellPrice;
}

int ItemData::GetMaxStack() const
{
	return m_maxStack;
}

const std::string& ItemData::GetUseActionID() const
{
	return m_useActionID; 
}