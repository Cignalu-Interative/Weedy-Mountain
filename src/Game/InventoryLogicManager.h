#pragma once

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


class InventoryLogic
{
public:

    enum class InventoryType
    {
        Basic, //tout ce qui est pas une hotbar ou l'inventaire du joueur
        Hotbar, //forcement au joueur (un marchand peut pas avoir de hotbar par exemple)
        PlayerInventory
    };
private:

    struct InventoryInfo
    {
        uint8_t flags;

        std::vector<InventorySlot*>* slots;

        InventoryType type;
    };

    std::vector<InventoryInfo> m_inventories;
    std::vector<InventoryInfo*> m_activeInventories; 

    ItemStack m_cursorStack;



    struct DraggedSlot
    {
        InventorySlot* slot;

        int originalAmount;

        bool wasEmptyAtDragStart;
    };

    std::vector<DraggedSlot> m_dragSlots;

    std::string m_dragItemID;

    int m_dragTotalAmount = 0;

public:

    InventoryLogic() = default;


    void AddInventory(std::vector<InventorySlot*>& slots, uint8_t flags, InventoryType type);

    void ClearInventories();


    ItemStack& GetCursorStack();

    bool HasCursorItem();


    void PickupStack(InventorySlot* slot);

    void PlaceStack(InventorySlot* slot);

    void SplitStack(InventorySlot* slot);

    void MergeStacks(InventorySlot* slot);

    void SwapStacks(InventorySlot* slot);

    void PlaceSingleItem(InventorySlot* slot);


    //Dragging (c'est compliqué o_O)
    void BeginDrag();

    void AddDragSlot(InventorySlot* slot);

    void CommitDrag();

    void CancelDrag();

    void DropSlot(InventorySlot* slot);

    void GatherAllItems(InventorySlot* slot);



    void ActivateInventory(std::vector<InventorySlot*>& slots);
    void DisactivateInventory(std::vector<InventorySlot*>& slots);


private:

    bool HasFlag(uint8_t flags, InventoryInterractionBit flag);

    InventoryInfo* FindInventory(InventorySlot* slot);

    bool CanInteract(InventorySlot* slot, InventoryInterractionBit flag);

    int GetMaxStack(const std::string& id);


    void DoPlaceStacks(InventorySlot* slot);

    void DoMergeStacks(InventorySlot* slot);

    void DoSwapStacks(InventorySlot* slot);




};