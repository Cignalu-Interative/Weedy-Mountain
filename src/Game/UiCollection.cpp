#include "pch.h"
#include "UiCollection.h"
#include "SpriteContainer.h"
#include "GameData.h"
#include "ItemDataBase.h"

void UiCollection::OnInitialize()
{
	RemoveCollider("default");

	AddSprite("backgroundCollection0", "collection0", {}, 0, {}, true);
	AddSprite("backgroundCollection1", "collection1", {}, 1, {}, true);

	GetSprite("backgroundCollection0")->ResizeStretched(GetDimension());
	GetSprite("backgroundCollection0")->SetPosition(GetPosition());
	GetSprite("backgroundCollection1")->ResizeStretched(GetDimension() + Vector2i(0,100));
	GetSprite("backgroundCollection1")->SetPosition(GetPosition());
	SyncSprites();

	SetDrawLayer(INT_MAX - 1);

	Vector2f nextPos = GetPosition({0.75,0.82}) + Vector2f(100,0);
	Vector2f previousPos = GetPosition({0.25, 0.82 }) + Vector2f(-100, 0);

	m_next = GetScene()->CreateButton(nextPos, { 32,32 }, "nextButton", "previousButton", "");
	m_next->SetDrawLayer(INT_MAX);
	m_next->ShowCollider(true);

	m_previous = GetScene()->CreateButton(previousPos, { 32,32 }, "previousButton", "nextButton", "");
	m_previous->SetDrawLayer(INT_MAX);
	m_previous->ShowCollider(true);



}

void UiCollection::OnUpdate()
{

}

void UiCollection::OnEvent()
{

}

void UiCollection::UpdateItems()
{
	for (std::string id : GameData::Get().GetPlayerData().m_unlockedItemsId)
	{
		ItemDataBase::Get()->GetItem(id);
		InventorySlot* slot = GetScene()->CreateEntity<InventorySlot>(m_slotStartPos + Vector2f(), {32,32}, "", Shape::Rectangle, true);
	}
}
