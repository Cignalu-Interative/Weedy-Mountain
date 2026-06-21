#include "pch.h"

#include "UiShop.h"
#include "Player.h"
#include "GameScene.h"

void UiShop::OnInitialize()
{
	GameScene* gameScene = GetScene<GameScene>();
	Scene* scene = GetScene();

	m_playerInventory = gameScene->GetPlayer()->GetInventory();

	Vector2f topRightPos = GetPosition({1,0});
	Vector2i dimension = GetDimension();
	float offsetX = dimension.GetX() / 5.f;
	float offsetY = dimension.GetY() / 4.f;
	Vector2f offset = topRightPos + Vector2f{ -offsetX, offsetY };

	m_slot = scene->CreateEntity<InventorySlot>(offset, {200,150}, "Oak_Tree", Shape::Rectangle);
	m_slot->ShowCollider(true);

	float offsetButtonX = GetPosition({ 1,1 }).GetX() - 175.f;
	float offsetButtonY = GetPosition({ 1,1 }).GetY() - 100.f;

	m_sellButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, {150,50}, "Oak_Tree", "sellButton", "SELL", true);
	m_sellButton->SetOnLeftClick([] {std::cout << "SELL ITEM" << std::endl; });
	m_sellButton->ShowCollider(true);

	//m_buyButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, { 150,50 }, "Oak_Tree", "buyButton", "BUY", true);
	//m_buyButton->SetOnLeftClick([] {std::cout << "BUY ITEM" << std::endl; });
	//m_buyButton->ShowCollider(true);
	Vector2f topLeftPos = GetPosition({ 0,0 });
	float BoffsetX = dimension.GetX() / 6.f;
	float BoffsetY = dimension.GetY() / 6.f ;
	Vector2f offsetPlayerInventory = topLeftPos + Vector2f{ BoffsetX, BoffsetY };

	m_choosePlayerInventory = scene->CreateButton({ offsetPlayerInventory }, { 150,50 }, "", "player", "Player Inventory");
	m_choosePlayerInventory->ShowCollider(true);
	m_choosePlayerInventory->SetOnLeftClick([this] {PlaceInventory(m_playerInventory); });
	//float offsetPriceX = GetPosition({ 0.5,1 }).GetX() - 200;
	//float offsetPriceY = GetPosition({ 0.5,1 }).GetY();

	//Vector2f posItemAmountText = m_inventory->GetPosition() - Vector2f{ 0, m_inventory->GetDimension().GetY() / 2 - 300 };
	//float amountOfItem = m_inventory->GetSlots()

	//scene->CreateText("totalPrice", { offsetPriceX,offsetPriceX }, std::to_string(amountOfItem /*item price*/), ColorsNamed::White);
	//scene->CreateText("itemAmount", { offsetPriceX,offsetPriceX }, std::to_string(amountOfItem ), ColorsNamed::White);
}

void UiShop::PlaceInventory(Inventory* p_inventory)
{
	Vector2f pos = GetPosition({0,0.5f});

	p_inventory->SetPosition(pos, {0,1});
}
