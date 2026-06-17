#include "pch.h"
#include "ItemStack.h"

ItemStack::ItemStack(const std::string& ID, int amount) : m_ID(std::move(ID)), m_amount(amount) {}

bool ItemStack::IsEmpty() const
{
	return m_amount == 0 || m_ID == "ITEM_NOTHING";
}

void ItemStack::Clear()
{
	m_amount = 0;
	m_ID = "ITEM_NOTHING";
}

const std::string& ItemStack::GetID() const { return m_ID; }
int ItemStack::GetAmount() const { return m_amount; }

void ItemStack::SetID(const std::string& newID) { m_ID = std::move(newID); }
void ItemStack::SetAmount(int newAmount) { m_amount = newAmount; }

