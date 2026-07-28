#include "pch.h"
#include "UiChest.h"
#include "SpriteContainer.h"
#include "GameData.h"
#include "Inventory.h"
#include "2DPendentLib/InputManager.h"


void UiChest::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);
	AddSprite("ChestBackground", "InventoryWeedyMountain", GetDimension(), 0,{}, true);
	GetSprite("ChestBackground")->ResizeStretched(GetDimension());
	SyncSprites();
	SetDrawLayer(INT_MAX - 200);
	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;
	

	Vector2f chestPos = GetScene()->GetWindowCenter() - Vector2f{0, 150};
	m_chestInventory = GetScene()->CreateEntity<Inventory>({ chestPos }, {636, 210}, "InventoryWeedyMountain", Shape::Rectangle,true);
	m_chestInventory->RemoveCollider("default");
	
	Vector2f textChestPos = chestPos - Vector2f{ 290,120 };
	Debug::Get()->CreateText("Chest", textChestPos, "CHEST", ColorsNamed::DarkBrown, true);

	Vector2f textInventoryPos = textChestPos + Vector2f{25, 240};
	Debug::Get()->CreateText("InventoryText", textInventoryPos, "INVENTORY", ColorsNamed::DarkBrown, true);
}

void UiChest::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
	{
		AddItemInChest();
		AddItemInPlayerInventory();
	}
}

void UiChest::AddItemInChest()
{

	std::vector<InventorySlot*> playerSlots = m_playerInventory->GetAllSlots();
	for (InventorySlot* slot : playerSlots)
	{
		if (slot->IsMouseIn() == false)
			continue;

		//Get item in playerInventory slot and add to chestInventory
		ItemStack stack = slot->GetStack();
		if(m_chestInventory->CanAdd(stack.GetID(), stack.GetAmount()))
		{
			m_chestInventory->AddItem(stack.GetID(), stack.GetAmount(), false);
			slot->Clear();
		}
	}
}

void UiChest::AddItemInPlayerInventory()
{
	std::vector<InventorySlot*> chestSlots = m_chestInventory->GetSlots();
	for (InventorySlot* slot : chestSlots)
	{
		if (slot->IsMouseIn() == false)
			continue;

		//Get item in playerInventory slot and add to chestInventory
		ItemStack stack = slot->GetStack();
		if (m_playerInventory->CanAdd(stack.GetID(), stack.GetAmount()))
		{
			m_playerInventory->AddItem(stack.GetID(), stack.GetAmount(), false);
			slot->Clear();
		}
	}
}

void UiChest::Open()
{
	Vector2f pos = GetPosition();

	float offsetY = pos.GetY() + 90;
	m_playerInventory->GetInventory()->SetPosition({ pos.GetX() ,offsetY });
	m_playerInventory->GetInventory()->SetVisible(true);

	m_playerInventory->GetHotbar()->SetPosition({ pos.GetX(),offsetY + 150 });
	m_playerInventory->GetHotbar()->SetIsVisible(true);

	m_chestInventory->SetVisible(true);

	Debug::Get()->GetText("Chest")->SetIsVisible(true);
	Debug::Get()->GetText("InventoryText")->SetIsVisible(true);
}

void UiChest::Close()
{
	SetIsVisible(false);
	m_chestInventory->SetVisible(false);

	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{ 0, 300 };
	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());

	Debug::Get()->GetText("Chest")->SetIsVisible(false);
	Debug::Get()->GetText("InventoryText")->SetIsVisible(false);
}
