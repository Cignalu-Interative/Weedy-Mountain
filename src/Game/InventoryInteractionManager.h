//certains noms sont bien pourris, si vous avez des suggestions, mp discord xD

#pragma once

#include "InventorySlot.h"
#include "ItemStack.h"

#include <vector>



class InventorySlot;

enum InventoryInterractionBit
{
	PickItem			= 1 << 0,
	PlaceItem			= 1 << 1,
	SplitStack			= 1 << 2,
	DragDistribute		= 1 << 3,
	DoubleClickGather	= 1 << 4,
	Drop				= 1 << 5,
	SwapItem			= 1 << 6,
	MergeStack			= 1 << 7
};

class InventoryManager
{
private:

	
	struct InventoryWithFlag //Le nom est pas terrible à changer
	{
		uint8_t m_flags;

		std::vector<InventorySlot*>* m_slots;
	};


	std::vector<InventoryWithFlag> m_iventoriesWithFlag;

	ItemStack m_cursorStack;

	bool m_isDragging = false;

	std::vector<InventorySlot*> m_alreadyDraggedSlots;




public:

	void AddInventory(std::vector<InventorySlot*>& slots, uint8_t bit);

	void ClearInventories();

	ItemStack& GetCursorStack();

	bool HasCursorItem();

	void HandleLeftClick(InventorySlot* slot);

	void HandleRightClick(InventorySlot* slot);

	void HandleDoubleLeftClick(InventorySlot* slot);

	void HandleDropOutside();

	void HandleLeftHeld(InventorySlot* slot);

	void HandleLeftRelease();


private:

	bool HasFlag(uint8_t flags, InventoryInterractionBit flag);

	InventoryWithFlag* FindInventory(InventorySlot* slot);

	void PickupStack(InventorySlot* slot);

	void PlaceStack(InventorySlot* slot);

	void SwapStacks(InventorySlot* slot);

	void MergeStacks(InventorySlot* slot);

	void SplitStack(InventorySlot* slot);

	void PlaceSingleItem(InventorySlot* slot);

	void GatherAllStacks(InventorySlot* slot);

	int GetMaxStack(const std::string& itemID);

	bool CanInteract(InventorySlot* slot, InventoryInterractionBit flag);
};