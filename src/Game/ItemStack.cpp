#include "pch.h"
#include "ItemStack.h"

ItemStack::ItemStack(ItemID ID, int amount)
	: m_IDD(ID), m_amount(amount) {}



bool ItemStack::IsEmpty()
{
	return m_amount == 0 || m_IDD == ITEM_NOTHING;
}

void ItemStack::Clear()
{
	m_amount = 0;
	m_IDD = ITEM_NOTHING;
}



ItemID ItemStack::GetID()
{
	return m_IDD;
}

void ItemStack::SetID(ItemID newID)
{
	m_IDD = newID;
}



int ItemStack::GetAmount()
{
	return m_amount;
}

void ItemStack::SetAmount(int newAmount)
{
	m_amount = newAmount;
}
