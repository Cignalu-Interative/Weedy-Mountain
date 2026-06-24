#include "pch.h"

#include "UiShop.h"
#include "Player.h"
#include "GameScene.h"
#include "2DPendentLib/InputManager.h"
#include "ItemDataBase.h"
#include "SpriteContainer.h"

void UiShop::OnInitialize()
{
	GameScene* gameScene = GetScene<GameScene>();
	Scene* scene = GetScene();

	m_playerInventory = gameScene->GetPlayer()->GetInventory();
	m_shopInventory = gameScene->GetPlayer2()->GetInventory();

	Vector2f topRightPos = GetPosition({ 1,0 });
	Vector2i dimension = GetDimension();
	float offsetX = dimension.GetX() / 5.f;
	float offsetY = dimension.GetY() / 4.f;
	Vector2f offset = topRightPos + Vector2f{ -offsetX, offsetY };

	m_slot = scene->CreateEntity<InventorySlot>(offset, { 200,150 }, "", Shape::Rectangle, true);
	//m_slot->ShowCollider(true);
	m_slot->GetCollider()->SetIsTrigger(true);

	m_slot->AddSprite("item_texture", "", m_slot->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_slot->SetSprite("item_texture", "");
	m_slot->GetSprite()->SetDimension(m_slot->GetDimension());
	m_slot->SyncSprites();

	m_slot->InitText("slot", "");


	float offsetButtonX = GetPosition({ 1,1 }).GetX() - offsetX;
	float offsetButtonY = GetPosition({ 1,1 }).GetY() - offsetY * 0.75f;

	m_sellButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, { 150,50 }, "", "sellButton", "SELL", true);
	m_sellButton->SetOnLeftClick([this] { Sell(); });
	//m_sellButton->ShowCollider(true);
	m_sellButton->GetCollider()->SetIsTrigger(true);
	m_sellButton->SetVisible(false);

	m_buyButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, { 150,50 }, "", "buyButton", "BUY", true);
	m_buyButton->SetOnLeftClick([this] {Buy(); });
	//m_buyButton->ShowCollider(true);
	m_buyButton->GetCollider()->SetIsTrigger(true);
	m_buyButton->SetVisible(false);

	Vector2f topLeftPos = GetPosition({ 0,0 });
	float BoffsetX = dimension.GetX() / 6.f;
	float BoffsetY = dimension.GetY() / 3.f;
	float BShopOffestX = dimension.GetX() / 2.9f;
	Vector2f offsetPlayerInventory = topLeftPos + Vector2f{ BoffsetX, BoffsetY };
	Vector2f offsetShopInventory = topLeftPos + Vector2f{ BShopOffestX, BoffsetY };

	m_choosePlayerInventory = scene->CreateButton({ offsetPlayerInventory }, { 150,50 }, "", "player", "Player Inventory");
	//m_choosePlayerInventory->ShowCollider(true);
	m_choosePlayerInventory->SetOnLeftClick([this] {ChoosePlayerInventoryButtonClicked(); });
	m_choosePlayerInventory->GetCollider()->SetIsTrigger(true);

	m_chooseShopInventory = scene->CreateButton({ offsetShopInventory }, { 150,50 }, "", "shop", "Shop Inventory");
	//m_chooseVendorInventory->ShowCollider(true);
	m_chooseShopInventory->SetOnLeftClick([this] {ChooseShopInventoryButtonClicked(); });
	m_chooseShopInventory->GetCollider()->SetIsTrigger(true);

	Vector2f halfRightPos = GetPosition({ 1,0.5f });
	float offsetPriceX = GetPosition().GetX();
	float offsetPriceY = halfRightPos.GetY() * 1.275f;
	float offsetItemNameY = halfRightPos.GetY() * 0.86f;

	scene->CreateText("itemAmount", { offset.GetX(),halfRightPos.GetY() }, "Item : " + std::to_string(m_amountOfItem), ColorsNamed::White, true);
	scene->CreateText("totalPrice", { offset.GetX(),offsetPriceY }, "Price : " + std::to_string(m_totalPrices), ColorsNamed::White, true);
	scene->CreateText("itemName", { offset.GetX(), offsetItemNameY },"", ColorsNamed::White, true);

	Vector2f plusButtonOffset = { GetPosition({ 1,0.5f }).GetX() - GetDimension().GetX() / 8.f, GetPosition({ 1,0.5f }).GetY() };
	Vector2f minusButtonOffset = { GetPosition({ 1,0.5f }).GetX() - GetDimension().GetX() / 3.62f, GetPosition({ 1,0.5f }).GetY() };

	m_plusButton = scene->CreateButton(plusButtonOffset, { 50,50 }, "", "plusButton", "+");
	m_plusButton->SetOnLeftClick([this] {PlusButton(); });
	//m_plusButton->ShowCollider(true);
	m_plusButton->GetCollider()->SetIsTrigger(true);

	m_minusButton = scene->CreateButton(minusButtonOffset, { 50,50 }, "", "minusButton", "-");
	m_minusButton->SetOnLeftClick([this] {MinusButton(); });
	//m_minusButton->ShowCollider(true);
	m_minusButton->GetCollider()->SetIsTrigger(true);

	GetCollider()->SetIsTrigger(true);
}

void UiShop::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		PlaceItemInInventoryToSlot();

	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT) && m_slot->IsMouseIn())
		m_slot->Clear();

	if (InputManager::Get()->IsKeyboardDown(K_n))
		SetVisible(!GetIsVisible());
}

void UiShop::OnUpdate()
{
	int price = 0;
	m_amountOfItem = m_slot->GetStack().GetAmount();

	if (m_sellButton->GetIsVisible() && m_slot->IsEmpty() == false)
		price = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetSellPrice();

	if (m_buyButton->GetIsVisible() && m_slot->IsEmpty() == false)
		price = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetBuyPrice();

	m_totalPrices = m_slot->GetStack().GetAmount() * price;

	GetScene()->GetGeometry<Text>("itemAmount")->SetText("Item : " + std::to_string(m_amountOfItem));
	GetScene()->GetGeometry<Text>("totalPrice")->SetText("Price : " + std::to_string(m_totalPrices));
	if (m_slot->IsEmpty())
	{
		GetScene()->GetGeometry<Text>("itemName")->SetText("");
		return;
	}
	std::string itemName = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetName();
	GetScene()->GetGeometry<Text>("itemName")->SetText(itemName);
}

void UiShop::PlaceInventory(Inventory* p_inventory)
{
	if (p_inventory == nullptr)
		return;

	Vector2f pos = GetPosition({ 0,0.5f });

	float offsetY = pos.GetY() + 150;
	p_inventory->SetPosition({ pos.GetX() * 1.25f,offsetY }, { 0,1 });
	p_inventory->SetVisible(true);
}

void UiShop::Buy()
{
	std::string id = m_slot->GetStack().GetID();
	int amount = m_slot->GetStack().GetAmount();

	if (id == "ITEM_NOTHING")
	{
		std::cout << "No item selected" << std::endl;
		return;
	}

	if (m_playerInventory->CanAdd(id, amount) && GetScene<GameScene>()->GetPlayer()->GetMoney() >= m_totalPrices)
	{
		m_playerInventory->AddItem(id, amount, false);
		GetScene<GameScene>()->GetPlayer()->DecreaseMoney(m_totalPrices);
		m_slot->Clear();
	}
	else
		std::cout << "Not enough space in inventory" << std::endl;

}

void UiShop::Sell()
{
	std::string id = m_slot->GetStack().GetID();
	int amount = m_slot->GetStack().GetAmount();

	if (m_playerInventory->HasAmountOfItem(id, amount))
	{
		GameScene* scene = GetScene<GameScene>();
		if (scene == nullptr)
			return;
		scene->GetPlayer()->AddMoney(m_totalPrices);
		m_playerInventory->RemoveItem(id, amount, false);
		m_slot->Clear();

	}
	else
		std::cout << "Not enough item in inventory" << std::endl;

}

void UiShop::ChoosePlayerInventoryButtonClicked()
{
	PlaceInventory(m_playerInventory);
	m_sellButton->SetVisible(true);
	m_buyButton->SetVisible(false);
	m_slot->Clear();
}

void UiShop::ChooseShopInventoryButtonClicked()
{
	PlaceInventory(m_shopInventory);
	m_buyButton->SetVisible(true);
	m_sellButton->SetVisible(false);
	m_playerInventory->SetPosition({ -500,0 });//TO do setPos du player
	m_slot->Clear();
}

void UiShop::PlaceItemInInventoryToSlot()
{
	Inventory* inventory = nullptr;

	if (m_sellButton->GetIsVisible())
		inventory = m_playerInventory;
	else
		inventory = m_shopInventory;
	//Get item of slot in the inventory and place it in the slot
	std::vector<InventorySlot*> slots = inventory->GetSlots();

	for (int i = 0; i < slots.size(); i++)
	{
		if (slots[i]->IsMouseIn() && slots[i]->IsEmpty() == false)
		{
			ItemStack item = slots[i]->GetStack();
			m_slot->SetText(std::to_string(item.GetAmount()));
			m_slot->SetStack(item);
			m_slotIndex = i;
		}
	}
}

void UiShop::PlusButton()
{
	if (m_slot->IsEmpty())
		return;

	if (m_amountOfItem == m_playerInventory->GetSlot(m_slotIndex)->GetStack().GetAmount())
		return;

	int amount = m_slot->GetStack().GetAmount();
	m_slot->GetStack().SetAmount(amount + 100);
	m_slot->SetText(std::to_string(m_slot->GetStack().GetAmount()));
}

void UiShop::MinusButton()
{
	if (m_slot->IsEmpty())
		return;

	int amount = m_slot->GetStack().GetAmount();
	int result = amount - 1;
	if (result <= 0)
	{
		result = 1;
	}
	m_slot->GetStack().SetAmount(result);
	m_slot->SetText(std::to_string(m_slot->GetStack().GetAmount()));
}

void UiShop::SetVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);

	m_choosePlayerInventory->SetVisible(p_isVisible);
	m_chooseShopInventory->SetVisible(p_isVisible);
	m_plusButton->SetVisible(p_isVisible);
	m_minusButton->SetVisible(p_isVisible);
	m_sellButton->SetVisible(false);
	m_buyButton->SetVisible(false);
	m_slot->SetVisible(p_isVisible);

	m_playerInventory->SetVisible(false);
	m_shopInventory->SetVisible(false);

	GetScene()->GetGeometry<Text>("itemAmount")->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("totalPrice")->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("itemName")->SetIsVisible(p_isVisible);
}

