#include "pch.h"
#include "InventoryInputControler.h"
#include "InventorySlot.h"
#include "InventoryStateMachine.h"

#include <iostream>

void InventoryInputController::Register(std::vector<InventorySlot*>& slots)
{
    for (InventorySlot* slot : slots)
    {
		slot->SetOnLeftClick([this, slot]() { OnLeftClick(slot); });
		slot->SetOnRightClick([this, slot]() { OnRightClick(slot);  });

		slot->SetOnLeftHeldEnter([this, slot]() { OnLeftHeldEnter(slot); });
		slot->SetOnRightHeldEnter([this, slot]() { OnRightHeldEnter(slot); });
		slot->SetOnLeftHeldExit([this, slot]() { OnLeftHeldExit(slot); });
		slot->SetOnRightHeldExit([this, slot]() { OnRightHeldExit(slot); });

		slot->SetOnReleaseLeftClick([this, slot]() { OnLeftRelease(slot); });
		slot->SetOnReleaseRightClick([this, slot]() { OnRightRelease(slot); });

		slot->SetOnDoubleLeftClick([this, slot]() { OnDoubleLeftClick(slot); });// Faire doubleRIght aussi
		slot->SetOnDoubleRightClick([this, slot]() { OnDoubleRightClick(slot); });// Faire doubleRIght aussi

		slot->SetOnMiddleClick([this, slot]() { OnMiddleClick(slot); });
    }
}

void InventoryInputController::SetStateMachine(InventoryStateMachine* stateMachine)
{
    m_stateMachine = stateMachine;
}

void InventoryInputController::OnLeftClick(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    //std::cout << "OnLeftClick\n";

    m_stateMachine->LeftClick(slot);
}

void InventoryInputController::OnRightClick(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->RightClick(slot);

    //std::cout << "OnRightClick\n";
}

void InventoryInputController::OnLeftHeldEnter(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->LeftHeldEnter(slot);

    //std::cout << "OnLeftHeldEnter\n";
}

void InventoryInputController::OnRightHeldEnter(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->RightHeldEnter(slot);

    //std::cout << "OnRightHeldEnter\n";
}

void InventoryInputController::OnLeftHeldExit(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->LeftHeldExit(slot);

    //std::cout << "OnLeftHeldExit\n";
}

void InventoryInputController::OnRightHeldExit(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->RightHeldExit(slot);

    //std::cout << "OnRightHeldExit\n";
}

void InventoryInputController::OnLeftRelease(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->LeftRelease(slot);

    //std::cout << "OnLeftRelease\n";
}

void InventoryInputController::OnRightRelease(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->RightRelease(slot);

    //std::cout << "OnRightRelease\n";
}

void InventoryInputController::OnDoubleLeftClick(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->DoubleLeftClick(slot);

    //std::cout << "OnDoubleLeftClick\n";
}

void InventoryInputController::OnDoubleRightClick(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->DoubleRightClick(slot);

}

void InventoryInputController::OnMiddleClick(InventorySlot* slot)
{
    if (m_stateMachine == nullptr)
        return;

    m_stateMachine->MiddleClick(slot);
}