// UiCollection.cpp
#include "pch.h"
#include "UiCollection.h"
#include "CollectionBlock.h"
#include "ItemDataBase.h"
#include "RecipeDataBase.h"
#include "GameScene.h"
#include "SpriteContainer.h"

const float spacingX = 110;
const float spacingY = 210;

void UiCollection::OnInitialize()
{
	RemoveCollider("default");
	SetDrawLayer(INT_MAX - 2);

	AddSprite("background", "InventoryWeedyMountain", GetDimension(), 0, {}, true);
	GetSprite("background")->SetDimension(GetDimension());
	GetSprite("background")->SetPosition(GetPosition());


	for (const auto& [id, data] : ItemDataBase::Get()->GetAll())
		m_allItemIDs.push_back(id);

	BuildGrid();

	Vector2f prevPos = { GetPosition({ 0.5f,1 }).GetX() - 400.f, GetPosition({ 0.5f,1 }).GetY() - 70 };
	Vector2f nextPos = { GetPosition({ 0.5f,1 }).GetX() + 400.f, GetPosition({ 0.5f,1 }).GetY() - 70 };
	Vector2f backPos = { GetPosition({ 0,0 }).GetX() + 45.f, GetPosition({ 0,0 }).GetY() + 60.f };

	m_itemPageName = Debug::Get()->CreateText("ItemNamePage", {GetPosition({0.15f,0.50f})}, "", ColorsNamed::Black, true);
	m_itemPageName->SetIsVisible(false);

	m_itemPageDescription = Debug::Get()->CreateText("ItemNameDescription", {GetPosition({0.10f,0.60f})}, "", ColorsNamed::Black, true);
	m_itemPageDescription->SetIsVisible(false);

	m_itemPageRecipies = Debug::Get()->CreateText("ItemNameRecipies", {GetPosition({0.50f,0.20f})}, "", ColorsNamed::Black, true);
	m_itemPageRecipies->SetIsVisible(false);

	m_prevPageButton = GetScene()->CreateButton(prevPos, { 50,50 }, "previousButton", "collectionPrevPage", "", true);
	m_prevPageButton->SetOnLeftClick([this] { PreviousPage(); });
	m_prevPageButton->GetCollider()->SetIsTrigger(true);
	m_prevPageButton->GetCollider()->SetMask(0);

	m_nextPageButton = GetScene()->CreateButton(nextPos, { 50,50 }, "nextButton", "collectionNextPage", "", true);
	m_nextPageButton->SetOnLeftClick([this] { NextPage(); });
	m_nextPageButton->GetCollider()->SetIsTrigger(true);
	m_nextPageButton->GetCollider()->SetMask(0);

	m_backButton = GetScene()->CreateButton(backPos, { 50,50 }, "crossButton", "backButton", "", true);
	m_backButton->SetIsVisible(false);
	m_backButton->SetOnLeftClick(
		[this] {
			m_backButton->SetIsVisible(false);
			m_isItemSelected = false;

			RemoveSprite("itemPageIcon");
			m_itemPageName->SetIsVisible(false);
			m_itemPageDescription->SetIsVisible(false);

			m_prevPageButton->SetIsVisible(true);
			m_nextPageButton->SetIsVisible(true);

			m_itemPageRecipies->SetIsVisible(false);

			RefreshPage();
		});
	m_backButton->GetCollider()->SetIsTrigger(true);

	Close();
	RefreshPage();
}

void UiCollection::OnUpdate()
{
	if (m_isItemSelected)
		return;

	RefreshPage();
}

void UiCollection::BuildGrid()
{
	float startX = GetPosition({ 0, 0 }).GetX() + 100.f;
	float startY = GetPosition({ 0, 0 }).GetY() + 150.f;

	for (int i = 0; i < m_allItemIDs.size(); i++)
	{
		int row = i / m_columns;
		int col = i % m_columns;

		Vector2f pos = { startX + col * spacingX, startY + row * spacingY };

		ItemBlock* block = GetScene()->CreateButton<ItemBlock>(pos, { 100,200 }, "InventoryWeedyMountain", "collectionBlock_" + std::to_string(i), "", true);
		block->SetIndex(i);
		GetScene()->GetGeometry<Text>(block->GetTextID())->SetPosition(block->GetPosition());
		block->SetOnLeftClick([this, block]
			{
				OnItemClick(block);
			});
		block->SetVisible(false);
		m_blocks.push_back(block);
	}

}

void UiCollection::RefreshPage()
{
	int startIndex = m_currentPage * m_blocksPerPage;

	for (int i = 0; i < m_blocksPerPage; i++)
	{
		int itemIndex = startIndex + i;

		if (itemIndex >= static_cast<int>(m_allItemIDs.size()))
		{
			m_blocks[i]->SetVisible(false);
			continue;
		}
		
		m_blocks[i]->SetItem(m_allItemIDs[itemIndex]);

		if (!m_isOpen)
			return;

		m_blocks[i]->SetVisible(true);
	}
}

void UiCollection::ClearPage()
{
	int startIndex = m_currentPage * m_blocksPerPage;

	for (int i = 0; i < m_blocksPerPage; i++)
	{
		int itemIndex = startIndex + i;

		if (itemIndex >= static_cast<int>(m_allItemIDs.size()))
		{
			m_blocks[i]->SetVisible(false);
			continue;
		}

		m_blocks[i]->SetVisible(false);
	}
}

void UiCollection::PreviousPage()
{
	if (m_currentPage <= 0)
		return;

	m_currentPage--;
	RefreshPage();
}

void UiCollection::NextPage()
{
	int maxPage = (static_cast<int>(m_allItemIDs.size()) - 1) / m_blocksPerPage;

	if (m_currentPage >= maxPage)
		return;

	m_currentPage++;
	RefreshPage();
}

void UiCollection::OnItemClick(ItemBlock* block)
{
	if (!m_isOpen)
		return;

	if (!block->IsUnlocked())
		return;

	if (m_isItemSelected)
		return;


	ClearPage();
	m_backButton->SetIsVisible(true);
	m_isItemSelected = true;
	std::string path =ItemDataBase::Get()->GetItem(block->GetItemID())->GetPath();
	std::string name =ItemDataBase::Get()->GetItem(block->GetItemID())->GetName();
	std::string description =ItemDataBase::Get()->GetItem(block->GetItemID())->GetDescription();
	std::string recipies = " Used in following crafts : \n\n";


	std::vector <std::string> temp = RecipeDataBase::Get()->GetRecipesNameForItem(block->GetItemID());

	for (int i = 0; i < temp.size(); i++)
	{
		if (RecipeDataBase::Get()->GetRecipesNameForItem(block->GetItemID()).empty())
			break;

		recipies += " -" + RecipeDataBase::Get()->GetRecipesNameForItem(block->GetItemID())[i] + "\n";
	}

	m_prevPageButton->SetIsVisible(false);
	m_nextPageButton->SetIsVisible(false);
	
	AddSprite("itemPageIcon", path, { 200,200 }, 1, { -325,-150 }, true);

	m_itemPageName->SetText(name);
	m_itemPageName->SetIsVisible(true);

	m_itemPageRecipies->SetText(recipies);
	m_itemPageRecipies->SetPosition(GetPosition({ 0.50f,0.20f }) ,{0.5, 0});
	m_itemPageRecipies->SetIsVisible(true);

	m_itemPageDescription->SetText(description);
	m_itemPageDescription->SetIsVisible(true);

}

void UiCollection::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);

	m_prevPageButton->SetIsVisible(p_isVisible);
	m_nextPageButton->SetIsVisible(p_isVisible);

}

void UiCollection::Open()
{
	m_isOpen = true;

	SetIsVisible(true);
	GameData::Get().GetPlayerData().m_playerInventory->GetHotbar()->SetIsVisible(false);
	RefreshPage();
}

void UiCollection::Close()
{
	for (ItemBlock* block : m_blocks)
		block->SetVisible(false);

	GameData::Get().GetPlayerData().m_playerInventory->GetHotbar()->SetIsVisible(true);
	SetIsVisible(false);
	m_itemPageDescription->SetIsVisible(false);
	m_itemPageRecipies->SetIsVisible(false);

	m_isOpen = false;
	
}