#include "pch.h"
#include "CollectionBlock.h"
#include "GameData.h"
#include "PlayerCollection.h"
#include "SpriteContainer.h"

void ItemBlock::OnInitialize()
{
	GetCollider()->SetIsTrigger(true);
	SetDrawLayer(INT_MAX);
	//ShowCollider(true);

	GetSprite()->ResizeStretched(GetDimension());

}

void ItemBlock::SetItem(const std::string& itemID)
{
	m_itemID = itemID;

	ItemDisplayInfo info = GameData::Get().GetPlayerCollection().GetDisplayInfo(itemID);
	m_unlocked = info.m_unlocked;

	GetSprite("icon" + std::to_string(m_index))->ChangeTexture(info.m_iconPath);
	SetText(info.m_name);
}

void ItemBlock::SetIndex(int p_index)
{
	m_index = p_index;
	AddSprite("icon" + std::to_string(m_index), "", { 16,16 }, 15, {}, true);
	GetSprite("icon" + std::to_string(m_index))->SetPosition(GetPosition({ 0.2f, 0.1 }));
	GetSprite("icon" + std::to_string(m_index))->SetDimension({ 64,64 });
	
}

void ItemBlock::SetVisible(bool p_visible)
{
	SetAllSpritesVisible(p_visible);
	GetScene()->GetGeometry<Text>(GetTextID())->SetIsVisible(p_visible);
}
