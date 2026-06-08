#include "pch.h"
#include "ItemData.h"

ItemData::ItemData(ItemID ID, std::string path, std::string name, std::string description, int buyPrice, int sellPrice, int maxStack) 
	: m_ID(ID), m_path(path), m_name(name), m_description(description), m_buyPrice(buyPrice), m_sellPrice(sellPrice), m_maxStack(maxStack) {}



ItemID ItemData::GetID()
{
	return m_ID;
}

std::string& ItemData::GetPath()
{
	return m_path;
}

std::string& ItemData::GetName()
{
	return m_name;
}

std::string& ItemData::GetDescription()
{
	return m_description;
}

int ItemData::GetBuyPrice()
{
	return m_buyPrice;
}

int ItemData::GetSellPrice()
{
	return m_sellPrice;
}

int ItemData::GetMaxStack()
{
	return m_maxStack;
}