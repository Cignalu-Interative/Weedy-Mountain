#include "pch.h"
#include "UiChest.h"
#include "SpriteContainer.h"
#include "GameData.h"
#include "Inventory.h"

void UiChest::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);
	//AddSprite("ChestBackground", "InventoryWeedyMountain", GetDimension());
	//GetSprite("ChestBackground")->ResizeStretched(GetDimension());
	//SyncSprites();

	//m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;

	m_chestInventory = CreateEntity<Inventory>({GetScene()->GetWindowCenter()}, {636, 210}, "InventoryWeedyMountain", Shape::Rectangle);
	//m_chestInventory->GetSprite()->ResizeStretched(m_chestInventory->GetDimension());
	m_chestInventory->SyncSprites();
	m_chestInventory->RemoveCollider("default");
}

void UiChest::AddItemInChest()
{

}

void UiChest::AddItemInPlayerInventory()
{

}

void UiChest::Open()
{
	Vector2f pos = GetPosition({ 0,0.5f });

	float offsetY = pos.GetY() + 110;
	//m_playerInventory->GetInventory()->SetPosition({ pos.GetX() * 1.25f,offsetY }, { 0,1 });
	//m_playerInventory->GetInventory()->SetVisible(true);

	//m_playerInventory->GetHotbar()->SetPosition({ pos.GetX() * 1.25f,offsetY + 110 }, { 0,1 });
	//m_playerInventory->GetHotbar()->SetIsVisible(true);

	Vector2f pos2 = GetPosition({ 1,0.5 });
	m_chestInventory->SetPosition({ pos2.GetX() * 1.25f,offsetY }, { 1,0 });
	m_chestInventory->SetVisible(true);

}

void UiChest::Close()
{
	SetIsVisible(false);
	m_chestInventory->SetVisible(false);

	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{ 0, 300 };
	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
}
