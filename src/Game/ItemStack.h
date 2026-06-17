#pragma once
#include "2DPendentEngine/Entity.h"

class ItemStack
{
private:

	std::string m_ID = "ITEM_NOTHING";

	int m_amount = 0;

public:

	ItemStack(const std::string& ID, int amount);

	ItemStack() = default;

	bool IsEmpty() const;
	void Clear();

	const std::string& GetID() const;
	void SetID(const std::string& newID);

	int GetAmount() const;
	void SetAmount(int newAmount);
};