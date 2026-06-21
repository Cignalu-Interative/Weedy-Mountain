#include "pch.h"

#include "UiShop.h"
#include "Player.h"
#include "GameScene.h"
#include "2DPendentLib/InputManager.h"

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
	m_slot->GetCollider()->SetIsTrigger(true);

	float offsetButtonX = GetPosition({ 1,1 }).GetX() - 175.f;
	float offsetButtonY = GetPosition({ 1,1 }).GetY() - 100.f;

	m_sellButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, {150,50}, "Oak_Tree", "sellButton", "SELL", true);
	m_sellButton->SetOnLeftClick([this] { Sell(); });
	//m_sellButton->ShowCollider(true);
	m_sellButton->GetCollider()->SetIsTrigger(true);
	m_sellButton->SetIsVisible(false);

	
	m_buyButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, { 150,50 }, "Oak_Tree", "buyButton", "BUY", true);
	m_buyButton->SetOnLeftClick([this] {Buy(); });
	m_buyButton->ShowCollider(true);
	m_buyButton->GetCollider()->SetIsTrigger(true);
	m_buyButton->SetIsVisible(false);

	Vector2f topLeftPos = GetPosition({ 0,0 });
	float BoffsetX = dimension.GetX() / 6.f;
	float BoffsetY = dimension.GetY() / 6.f ;
	float BSoffestX = dimension.GetX() / 2.9f;
	Vector2f offsetPlayerInventory = topLeftPos + Vector2f{ BoffsetX, BoffsetY };
	Vector2f offsetShopInventory = topLeftPos + Vector2f{ BSoffestX, BoffsetY };

	m_choosePlayerInventory = scene->CreateButton({ offsetPlayerInventory }, { 150,50 }, "", "player", "Player Inventory");
	m_choosePlayerInventory->ShowCollider(true);
	m_choosePlayerInventory->SetOnLeftClick([this] {ChoosePlayerInventoryButtonClicked(); });
	m_choosePlayerInventory->GetCollider()->SetIsTrigger(true);

	m_chooseVendorInventory = scene->CreateButton({ offsetShopInventory }, { 150,50 }, "", "shop", "Shop Inventory");
	m_chooseVendorInventory->ShowCollider(true);
	m_chooseVendorInventory->SetOnLeftClick([this] {ChooseShopInventoryButtonClicked(); });
	m_chooseVendorInventory->GetCollider()->SetIsTrigger(true);

	Vector2f halfRightPos = GetPosition({ 1,0.5f });
	float offsetPriceX = GetPosition().GetX();
	float offsetPriceY = halfRightPos.GetY() + 100;

	//Vector2f posItemAmountText = m_inventory->GetPosition() - Vector2f{ 0, m_inventory->GetDimension().GetY() / 2 - 300 };
	//float amountOfItem = m_inventory->GetSlots();

	scene->CreateText("itemAmount", { offset.GetX(),halfRightPos.GetY() }, "Item : " + std::to_string(m_amountOfItem), ColorsNamed::White);
	scene->CreateText("totalPrice", { offset.GetX(),offsetPriceY }, "Price : " + std::to_string(m_totalPrices), ColorsNamed::White);

	GetCollider()->SetIsTrigger(true);
}

void UiShop::OnEvent()
{
	if(InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		PlaceItemInInventoryToSlot();
}

void UiShop::PlaceInventory(Inventory* p_inventory)
{
	if (p_inventory == nullptr)
		return;

	Vector2f pos = GetPosition({0,0.5f});

	float offsetY = pos.GetY() + 150;
	p_inventory->SetPosition({ pos.GetX(),offsetY }, { 0,1 });
}

void UiShop::Buy()
{
	std::string id = m_slot->GetStack().GetID();
	int amount = m_slot->GetStack().GetAmount();

	if(id == "ITEM_NOTHING")
	{
		std::cout << "No item selected" << std::endl;
		return;
	}

	if (m_playerInventory->CanAdd(id, amount))
		m_playerInventory->AddItem(id, amount, false);
	else
		std::cout << "Not enough space in inventory" << std::endl;

}

void UiShop::Sell()
{
	std::string id = m_slot->GetStack().GetID();
	int amount = m_slot->GetStack().GetAmount();

	if(m_playerInventory->HasAmountOfItem(id, amount))
	{
		GameScene* scene = GetScene<GameScene>();
		if (scene == nullptr)
			return;
		scene->GetPlayer()->AddMoney(m_totalPrices);
		m_playerInventory->RemoveItem(id, amount, false);
	}
	else
		std::cout << "Not enough item in inventory" << std::endl;

}

void UiShop::ChoosePlayerInventoryButtonClicked()
{
	PlaceInventory(m_playerInventory); 
	m_sellButton->SetIsVisible(true); 
	m_buyButton->SetIsVisible(false);
}

void UiShop::ChooseShopInventoryButtonClicked()
{
	PlaceInventory(m_shopInventory);
	m_buyButton->SetIsVisible(true); 
	m_sellButton->SetIsVisible(false);
}

void UiShop::PlaceItemInInventoryToSlot()
{
	//Get item of slot in the inventory and place it in the slot
	std::vector<InventorySlot*> slots = m_playerInventory->GetSlots();

	for (InventorySlot* slot : slots)
	{
		if (slot->IsMouseIn() && slot->IsEmpty() == false)
		{
			//ItemStack item =  slot->GetStack();
			//m_slot->SetStack(item);
		}
	}
}

