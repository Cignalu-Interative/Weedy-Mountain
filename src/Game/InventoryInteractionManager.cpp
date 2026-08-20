#include "pch.h"
#include "InventoryInteractionManager.h"
#include "ItemDataBase.h"
#include "2DPendentLib/InputManager.h"
#include "InventorySlot.h"


#include "Button.h"
#include "Entity.h"
#include "SpriteContainer.h"
#include "Hotbar.h"
#include "Inventory.h"
#include "PlayerInventory.h"
#include "DemoScene.h"

#include <iostream>


void InventoryManager::Initialize(Scene* scene, Inventory* inventory, Hotbar* hotbar, PlayerInventory* playerInventory)
{
    m_stateMachine.SetLogic(&m_logic);

    m_inputController.SetStateMachine(&m_stateMachine);

    m_scene = scene;

	m_cursorUI = scene->CreateButton<CursorItemUI>({ 0,0 }, { 54,54 }, "", "cursor_amount", "", true);
	m_cursorUI->SetStack(&m_logic.GetCursorStack());


    m_hotbar = hotbar;
    m_inventory = inventory;
    m_isInventoryOpen = false;
    Close();

    m_notificationManager.Initialize(scene);

    m_playerInventory = playerInventory;

    m_playerInventory->SetNotificationManager(&m_notificationManager);
}

void InventoryManager::RegisterInventory(std::vector<InventorySlot*>& slots, uint8_t flags, InventoryLogic::InventoryType type)
{
    m_inputController.Register(slots);

    m_logic.AddInventory(slots, flags, type);
}

InventoryLogic* InventoryManager::GetLogic()
{
	return &m_logic;
}

const char* InventoryManager::GetState()
{
    return m_stateMachine.GetState();
}

void InventoryManager::Open()
{
    m_isInventoryOpen = true;
    m_inventory->SetVisible(true);
    m_hotbar->SetPosition({ m_inventory->GetPosition().GetX(), m_inventory->GetPosition().GetY() + m_inventory->GetDimension().GetY() * 0.5f }, { 0.5f, 0.0f });
    m_hotbar->RealignSlots();
    m_hotbar->SetIsInventoryOpen(true);
}

void InventoryManager::Close()
{
    m_isInventoryOpen = false;
    m_inventory->SetVisible(false);
    m_hotbar->SetPosition({ m_inventory->GetPosition().GetX(), m_inventory->GetPosition().GetY() + 300 });
    m_hotbar->RealignSlots();
    m_hotbar->SetIsInventoryOpen(false);
}

void InventoryManager::Toggle()
{
    if (m_isInventoryOpen)
    {
        Close();
        m_logic.DisactivateInventory(m_hotbar->GetSlots());
        m_logic.DisactivateInventory(m_inventory->GetSlots());
    }
    else
    {
        Open();
        m_logic.ActivateInventory(m_hotbar->GetSlots());
        m_logic.ActivateInventory(m_inventory->GetSlots());        
    }
}

bool InventoryManager::IsOpen()
{
    return m_isInventoryOpen;
}

void InventoryManager::OnUpdate()
{
    InputManager* input = InputManager::Get();

    m_notificationManager.Update();

    if (input->IsKeyboardDown(K_TAB))
    {
        Toggle();
        return;
    }

    if (m_isInventoryOpen == false)
    {
        if (input->IsMouseScrollDown(SCROLL_UP))
            m_hotbar->GoNext(1);

        else if (input->IsMouseScrollDown(SCROLL_DOWN))
            m_hotbar->GoNext(-1);

        UpdateHotbarSelection(input);

        if (input->IsMouseDown(BUTTON_LEFT))
            SelectHoveredHotbarSlot();
        return;
    }
}

void InventoryManager::UpdateHotbarSelection(InputManager* input)
{
	if (input->IsKeyboardDown(K_1))
		m_hotbar->Select(0);
	if (input->IsKeyboardDown(K_2))
        m_hotbar->Select(1);
	if (input->IsKeyboardDown(K_3))
        m_hotbar->Select(2);
	if (input->IsKeyboardDown(K_4))
        m_hotbar->Select(3);
	if (input->IsKeyboardDown(K_5))
        m_hotbar->Select(4);
	if (input->IsKeyboardDown(K_6))
        m_hotbar->Select(5);
	if (input->IsKeyboardDown(K_7))
        m_hotbar->Select(6);
	if (input->IsKeyboardDown(K_8))
        m_hotbar->Select(7);
	if (input->IsKeyboardDown(K_9))
        m_hotbar->Select(8);
	if (input->IsKeyboardDown(K_0))
        m_hotbar->Select(9);
}

void InventoryManager::SelectHoveredHotbarSlot()
{
	int c = 0;

	for (InventorySlot* slot : m_hotbar->GetSlots())
	{
		if (slot->IsMouseIn())
		{
			m_hotbar->Select(c);
			break;
		}

		c++;
	}
}











void CursorItemUI::OnInitialize()
{
	AddSprite("cursorItemSprite", "", { 54,54 }, 1, { 0,0 }, true);
	InitText("cursor_amount", "");
    GetCollider()->SetIsTrigger(true);
}

void CursorItemUI::OnUpdate()
{

    Vector2f mousePOs = InputManager::Get()->GetMousePosition();

    SetPosition(mousePOs);



    if (m_stack == nullptr)
        return;

    RefreshVisual();
    
}

void CursorItemUI::SetStack(ItemStack* stack)
{
    m_stack = stack;
}

void CursorItemUI::RefreshVisual()
{

    if (m_stack->IsEmpty())
    {   
        SetSprite("cursorItemSprite", "");
        SetText("");
        return;
    }

    ItemData* data = ItemDataBase::Get()->GetItem(m_stack->GetID());

    if (data)
    {
        if (m_isVisible)
        {
            /*SetSprite("cursorItemSprite", data->GetPath());*/
            RemoveSprite("cursorItemSprite");
            AddSprite("cursorItemSprite", data->GetPath(), { 54,54 }, 1, { 0,0 }, true);
            GetSprite("cursorItemSprite")->Resize({ 54, 54 });
            SetDrawLayer(6000);
            SetText(std::to_string(m_stack->GetAmount()));
            SetTextOffset({ 18.0f, 22.0f });
        }
        else
        {
            SetSprite("cursorItemSprite", "");
            SetDrawLayer(6000);
            SetText("");
            SetTextOffset({ 18.0f, 22.0f });
        }
    }
}