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

	std::function<void()> m_onLeftHeldEnter;   
	std::function<void()> m_onRightHeldEnter; 
	std::function<void()> m_onLeftRelease;    
	std::function<void()> m_onRightRelease;   

	bool m_wasMouseIn = false;

public:

	InventorySlot();

	bool IsEmpty();
	void Clear();

	ItemStack& GetStack();
	void SetStack(const ItemStack& stack);

	void RefreshVisual();

	void SetOnLeftHeldEnter(std::function<void()> fn) { m_onLeftHeldEnter = fn; }
	void SetOnRightHeldEnter(std::function<void()> fn) { m_onRightHeldEnter = fn; }
	void SetOnLeftRelease(std::function<void()> fn) { m_onLeftRelease = fn; }
	void SetOnRightRelease(std::function<void()> fn) { m_onRightRelease = fn; }

	void OnUpdate() override;
};