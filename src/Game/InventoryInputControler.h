#pragma once

#include <vector>

class InventorySlot;
class InventoryStateMachine;

class InventoryInputController
{
private:

    InventoryStateMachine* m_stateMachine = nullptr;


public:

    InventoryInputController() = default;

    void Register(std::vector<InventorySlot*>& slots);

    void SetStateMachine(InventoryStateMachine* stateMachine);

private:

    void OnLeftClick(InventorySlot* slot);
    void OnRightClick(InventorySlot* slot);

    void OnLeftHeldEnter(InventorySlot* slot);
    void OnRightHeldEnter(InventorySlot* slot);

    void OnLeftHeldExit(InventorySlot* slot);
    void OnRightHeldExit(InventorySlot* slot);

    void OnLeftRelease(InventorySlot* slot);
    void OnRightRelease(InventorySlot* slot);

    void OnDoubleLeftClick(InventorySlot* slot);
    void OnDoubleRightClick(InventorySlot* slot);

    void OnMiddleClick(InventorySlot* slot);


};