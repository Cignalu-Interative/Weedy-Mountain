#pragma once

#include <string>
#include <vector>

class InventorySlot;
class InventoryLogic;

class InventoryStateMachine
{
private:

    enum class State
    {
        Idle,
        Holding,

        LeftPending,
        RightPending,

        LeftDrag,
        RightDrag
    };


    InventoryLogic* m_logic = nullptr;

    State m_state = State::Idle;

    InventorySlot* m_pressedSlot = nullptr;

public:

    InventoryStateMachine() = default;

    void SetLogic(InventoryLogic* logic);

    void LeftClick(InventorySlot* slot);
    void RightClick(InventorySlot* slot);

    void LeftHeldEnter(InventorySlot* slot);
    void RightHeldEnter(InventorySlot* slot);

    void LeftHeldExit(InventorySlot* slot);
    void RightHeldExit(InventorySlot* slot);

    void LeftRelease(InventorySlot* slot);
    void RightRelease(InventorySlot* slot);

    void DoubleLeftClick(InventorySlot* slot);
    void DoubleRightClick(InventorySlot* slot);

    void MiddleClick(InventorySlot* slot);

    const char* GetState() { return StateToString(m_state); }

private:

    void SetState(State state);
    const char* StateToString(State state);
};