#include "pch.h"
#include "InventoryStateMachine.h"
#include "InventoryLogicManager.h"
#include "InventorySlot.h"

#include <iostream>

void InventoryStateMachine::SetLogic(InventoryLogic* logic)
{
    m_logic = logic;
}

void InventoryStateMachine::LeftClick(InventorySlot* slot) //merci Minecraft (ou pas :/)
{
    if (m_logic == nullptr || slot == nullptr)
        return;

    switch (m_state)
    {
    case State::Idle:
    {
        if (slot->IsEmpty())
            return;

        m_logic->PickupStack(slot);
        SetState(State::Holding);

        break;
    }
    case State::Holding:
    {
        m_pressedSlot = slot;
        SetState(State::LeftPending);

        break;
    }
    default:
        break;
    }
}

void InventoryStateMachine::RightClick(InventorySlot* slot)
{
    if (m_logic == nullptr || slot == nullptr)
        return;

    switch (m_state)
    {
    case State::Idle:
    {
        if (slot->IsEmpty())
            return;

        m_logic->SplitStack(slot);
        SetState(State::Holding);

        break;
    }
    case State::Holding:
    {
        m_pressedSlot = slot;
        SetState(State::RightPending);

        break;
    }
    default:
        break;
    }
}

void InventoryStateMachine::LeftHeldEnter(InventorySlot* slot)
{
    if (m_logic == nullptr || slot == nullptr)
        return;

    switch (m_state)
    {
    case State::LeftPending:
    {
        m_logic->BeginDrag();
        m_logic->AddDragSlot(m_pressedSlot);
        m_logic->AddDragSlot(slot);

        SetState(State::LeftDrag);

        break;
    }
    case State::LeftDrag:
    {
        m_logic->AddDragSlot(slot);

        break;
    }
    default:
        break;
    }
}

void InventoryStateMachine::RightHeldEnter(InventorySlot* slot)
{
    if (m_logic == nullptr || slot == nullptr)
        return;

    switch (m_state)
    {
    case State::RightPending:
    {
        m_logic->PlaceSingleItem(m_pressedSlot);
        m_logic->PlaceSingleItem(slot);
        SetState(State::RightDrag);

        break;
    }
    case State::RightDrag:
    {
        m_logic->PlaceSingleItem(slot);

        break;
    }
    default:
        break;
    }
}

void InventoryStateMachine::LeftHeldExit(InventorySlot* slot)
{

}

void InventoryStateMachine::RightHeldExit(InventorySlot* slot)
{

}

void InventoryStateMachine::LeftRelease(InventorySlot* slot)
{
    if (m_logic == nullptr)
        return;

    switch (m_state)
    {
    case State::LeftPending:
    {
        if (slot == m_pressedSlot)
        {
            m_logic->PlaceStack(slot);
        }

        SetState(State::Holding);

        break;
    }
    case State::LeftDrag:
    {
        m_logic->CommitDrag();
        SetState(State::Holding);

        break;
    }
    default:
        break;
    }
    m_pressedSlot = nullptr;

    if (m_logic->HasCursorItem())
        SetState(State::Holding);
    else
        SetState(State::Idle);
}

void InventoryStateMachine::RightRelease(InventorySlot* slot)
{
    if (m_logic == nullptr)
        return;

    switch (m_state)
    {
    case State::RightPending:
    {
        if (slot == m_pressedSlot)
        {
            m_logic->PlaceSingleItem(slot);
        }

        SetState(State::Holding);

        break;
    }
    case State::RightDrag:
    {
        SetState(State::Holding);

        break;
    }
    default:
        break;
    }
    m_pressedSlot = nullptr;

    if (m_logic->HasCursorItem())
        SetState(State::Holding);
    else
        SetState(State::Idle);
}

void InventoryStateMachine::DoubleLeftClick(InventorySlot* slot)
{
    if (m_logic == nullptr || slot == nullptr)
        return;

    m_logic->GatherAllItems(slot);
    SetState(State::Holding);

    if (m_logic->HasCursorItem())
        SetState(State::Holding);
    else
        SetState(State::Idle);
}

void InventoryStateMachine::DoubleRightClick(InventorySlot* slot)
{

}

void InventoryStateMachine::MiddleClick(InventorySlot* slot)
{
	if (m_logic == nullptr || slot == nullptr)
		return;

	m_logic->DropSlot(slot);

}

void InventoryStateMachine::SetState(State state)
{
    m_state = state;

    std::cout << "State = " << StateToString(state) << std::endl;
}

const char* InventoryStateMachine::StateToString(State state)
{
    switch (state)
    {
    case State::Idle: return "Idle";
    case State::Holding: return "Holding";
    case State::LeftPending: return "LeftPending";
    case State::RightPending: return "RightPending";
    case State::LeftDrag: return "LeftDrag";
    case State::RightDrag: return "RightDrag";
    }

    return "ERROR ERRORERROR ERROR ERROR";
}
