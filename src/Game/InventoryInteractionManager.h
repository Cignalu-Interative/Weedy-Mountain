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

	enum class State
	{
		Idle,              
		Holding,           
		LeftPendingPlace,
		RightPendingPlace,
		LeftDragSplit,
		RightDragSplit,
	};
	
	struct InventoryWithFlag //Le nom est pas terrible à changer
	{
		uint8_t m_flags;

		std::vector<InventorySlot*>* m_slots;
	};


	std::vector<InventoryWithFlag> m_iventoriesWithFlag;

	ItemStack m_cursorStack;

	State m_state = State::Idle;

	InventorySlot* m_pressedSlot = nullptr;

	std::vector<InventorySlot*> m_dragSlots;
	std::vector<int> m_dragOriginalAmount;
	std::string m_dragItemID;
	int m_dragTotalAtStart = 0; 

	std::chrono::steady_clock::time_point m_lastPickupTime;
	float m_doubleClickDelay = 0.25f;
	InventorySlot* m_lastPickedSlot = nullptr;

public:

	void AddInventory(std::vector<InventorySlot*>& slots, uint8_t bit);
	void ClearInventories();

	ItemStack& GetCursorStack();
	bool HasCursorItem();

	void HandleDropOutside();

private:

	bool HasFlag(uint8_t flags, InventoryInterractionBit flag);
	InventoryWithFlag* FindInventory(InventorySlot* slot);
	bool CanInteract(InventorySlot* slot, InventoryInterractionBit flag);
	int  GetMaxStack(const std::string& id);

	void OnLeftDown(InventorySlot* slot);
	void OnRightDown(InventorySlot* slot);
	void OnLeftHeldEnter(InventorySlot* slot);
	void OnRightHeldEnter(InventorySlot* slot);
	void OnLeftRelease(InventorySlot* slot);
	void OnRightRelease(InventorySlot* slot);
	void OnDoubleLeftClick(InventorySlot* slot);

	void PickupStack(InventorySlot* slot);
	void PlaceFullStack(InventorySlot* slot);
	void SwapStacks(InventorySlot* slot);
	void MergeStacks(InventorySlot* slot);
	void SplitStackToCursor(InventorySlot* slot);   
	void PlaceSingleItem(InventorySlot* slot);

	void RedistributeDrag();                       
	void CommitDrag();                              
	void CancelDrag();                            
};