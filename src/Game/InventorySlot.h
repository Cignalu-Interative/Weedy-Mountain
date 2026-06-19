#pragma once

#include "Button.h"
#include "ItemStack.h"

#include <string>

class InventorySlot : public Button
{
private:

	ItemStack m_stack;

	std::string m_amountText;
	std::string m_iconPath;

public:

	InventorySlot();

	bool IsEmpty();
	void Clear();

	ItemStack& GetStack();
	void SetStack(const ItemStack& stack);

	void RefreshVisual();

	void OnUpdate() override;
};