#include "pch.h"

#include "UiShop.h"
#include "Player.h"
#include "GameScene.h"
#include "2DPendentLib/InputManager.h"
#include "ItemDataBase.h"
#include "SpriteContainer.h"
#include "TextBox.h"

#include "PlayerInventory.h"

void UiShop::OnInitialize()
{
	Scene* scene = GetScene();

	RemoveCollider("default");

	//m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory->GetInventory();
	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;

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
	m_slot->GetCollider()->SetMask(0);

	float offsetButtonX = GetPosition({ 1,1 }).GetX() - offsetX;
	float offsetButtonY = GetPosition({ 1,1 }).GetY() - offsetY * 0.85f;

	m_sellButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, { 150,50 }, "button", "sellButton", "SELL", true);
	m_sellButton->SetOnLeftClick([this] { Sell(); });
	m_sellButton->SetOnHoverStay([this] { m_sellButton->SetSprite("buttonHover"); });
	m_sellButton->SetOnHoverExit([this] { m_sellButton->SetSprite("button"); });
	m_sellButton->GetCollider()->SetIsTrigger(true);
	m_sellButton->SetIsVisible(false);
	m_sellButton->GetCollider()->SetMask(0);
	
	m_sellAllButton = scene->CreateButton({ offsetButtonX, offsetButtonY + 55}, { 150,50 }, "button", "sellAllButton", "SELL ALL", true);
	m_sellAllButton->SetOnHoverStay([this] { m_sellAllButton->SetSprite("buttonHover"); });
	m_sellAllButton->SetOnHoverExit([this] { m_sellAllButton->SetSprite("button"); });
	m_sellAllButton->SetOnLeftClick([this] { SellAll(); });
	m_sellAllButton->SetIsVisible(false);
	m_sellAllButton->GetCollider()->SetIsTrigger(true);
	m_sellAllButton->GetCollider()->SetMask(0);

	m_buyButton = scene->CreateButton({ offsetButtonX, offsetButtonY }, { 150,50 }, "button", "buyButton", "BUY", true);
	m_buyButton->SetOnLeftClick([this] {Buy(); });
	m_buyButton->SetOnHoverStay([this] { m_buyButton->SetSprite("buttonHover"); });
	m_buyButton->SetOnHoverExit([this] { m_buyButton->SetSprite("button"); });
	m_buyButton->GetCollider()->SetIsTrigger(true);
	m_buyButton->SetIsVisible(false);
	m_buyButton->GetCollider()->SetMask(0);

	Vector2f topLeftPos = GetPosition({ 0,0 });
	float BoffsetX = dimension.GetX() / 6.f;
	float BoffsetY = dimension.GetY() / 3.5f;
	float BShopOffestX = dimension.GetX() / 2.9f;
	Vector2f offsetPlayerInventory = topLeftPos + Vector2f{ BoffsetX, BoffsetY };
	Vector2f offsetShopInventory = topLeftPos + Vector2f{ BShopOffestX, BoffsetY };

	m_choosePlayerInventory = scene->CreateButton({ offsetPlayerInventory }, { 150,50 }, "", "player", "Player Inventory");
	m_choosePlayerInventory->SetOnLeftClick([this] {ChoosePlayerInventoryButtonClicked(); });
	m_choosePlayerInventory->GetCollider()->SetIsTrigger(true);
	m_choosePlayerInventory->GetCollider()->SetMask(0);

	m_chooseShopInventory = scene->CreateButton({ offsetShopInventory }, { 150,50 }, "", "shop", "Shop Inventory");
	m_chooseShopInventory->SetOnLeftClick([this] {ChooseShopInventoryButtonClicked(); });
	m_chooseShopInventory->GetCollider()->SetIsTrigger(true);
	m_chooseShopInventory->GetCollider()->SetMask(0);

	Vector2f halfRightPos = GetPosition({ 1,0.5f });
	float offsetPriceX = GetPosition().GetX();
	float offsetPriceY = halfRightPos.GetY() * 1.275f;
	float offsetItemNameY = halfRightPos.GetY() * 0.86f;

	//m_textBox = scene->CreateTextBox("itemAmount", { offset.GetX(),halfRightPos.GetY() }, { 50,25 }, ColorsNamed::White);
	scene->CreateText("itemAmount", { offset.GetX(),halfRightPos.GetY() }, "Item : " + std::to_string(m_amountOfItem), ColorsNamed::White, true);
	scene->CreateText("totalPrice", { offset.GetX(),offsetPriceY }, "Price : " + std::to_string(m_totalPrices), ColorsNamed::White, true);
	scene->CreateText("itemName", { offset.GetX(), offsetItemNameY },"", ColorsNamed::White, true);

	Vector2f plusButtonOffset = { GetPosition({ 1,0.5f }).GetX() - GetDimension().GetX() / 8.f, GetPosition({ 1,0.5f }).GetY() };
	Vector2f minusButtonOffset = { GetPosition({ 1,0.5f }).GetX() - GetDimension().GetX() / 3.62f, GetPosition({ 1,0.5f }).GetY() };

	m_plusButton = scene->CreateButton(plusButtonOffset, { 50,50 }, "", "plusButton", "+");
	m_plusButton->SetOnLeftClick([this] {PlusButton(); });
	m_plusButton->GetCollider()->SetIsTrigger(true);
	m_plusButton->GetCollider()->SetMask(0);

	m_minusButton = scene->CreateButton(minusButtonOffset, { 50,50 }, "", "minusButton", "-");
	m_minusButton->SetOnLeftClick([this] {MinusButton(); });
	m_minusButton->GetCollider()->SetIsTrigger(true);
	m_minusButton->GetCollider()->SetMask(0);

	SetDrawLayer(GetDrawLayer() - 20);

}

void UiShop::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		PlaceItemInInventoryToSlot();

	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT) && m_slot->IsMouseIn())
		m_slot->Clear();

	if (InputManager::Get()->IsKeyboardDown(K_N))
		SetIsVisible(!GetIsVisible());
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

	//m_textBox->SetText("Item : " + std::to_string(m_amountOfItem));
	GetScene()->GetGeometry<Text>("itemAmount")->SetText("Item : " + std::to_string(m_amountOfItem));
	GetScene()->GetGeometry<Text>("totalPrice")->SetText("Price : " + std::to_string(m_totalPrices));
	if (m_slot->IsEmpty())
	{
		GetScene()->GetGeometry<Text>("itemName")->SetText("");
		return;
	}
	std::string itemName = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetName();
	GetScene()->GetGeometry<Text>("itemName")->SetText(itemName);

	//std::cout << "Slot :" << m_playerInventory->GetSlot(0)->GetIsVisible() << std::endl;
	//std::cout << "Update Text :" <<GetScene()->GetGeometry<Text>(m_playerInventory->GetSlot(0)->GetTextID())->GetText() << std::endl;
}

void UiShop::PlaceInventory(Inventory* p_inventory)
{ 
	if (p_inventory == nullptr)
		return;

	Vector2f pos = GetPosition({ 0,0.5f });

	float offsetY = pos.GetY() + 110;
	p_inventory->SetPosition({ pos.GetX() * 1.25f,offsetY }, { 0,1 });
	p_inventory->SetVisible(true);

	if (p_inventory == m_playerInventory->GetInventory())
	{
		m_playerInventory->GetHotbar()->SetPosition({ pos.GetX() * 1.25f,offsetY + 110 }, { 0,1 });
		m_playerInventory->GetHotbar()->SetIsVisible(true);
	}
}

void UiShop::Buy()
{
	if (m_slot->IsEmpty())
		return;

	std::string id = m_slot->GetStack().GetID();
	int amount = m_slot->GetStack().GetAmount();

	if (m_playerInventory->CanAdd(id, amount) && GameData::Get().GetPlayerData().m_money >= m_totalPrices)
	{
		m_playerInventory->AddItem(id, amount, false);
		GameData::Get().GetPlayerData().m_money -= m_totalPrices;//GetScene<GameScene>()->GetPlayer()->DecreaseMoney(m_totalPrices);
		m_slot->Clear();
	}

}

void UiShop::Sell()
{
	if (m_slot->IsEmpty())
		return;

	std::string id = m_slot->GetStack().GetID();
	int amount = m_slot->GetStack().GetAmount();

	if(m_playerInventory->HasItems(id,amount))
	{
		GameData::Get().GetPlayerData().m_money += m_totalPrices;
		m_playerInventory->RemoveItem(id, amount, false);
		m_slot->Clear();
	}

	std::vector<InventorySlot*> slots = m_playerInventory->GetAllSlots();
	for (int i = 0; i < slots.size(); i++)
	{
		if (slots[i] == m_selectedSlot)
			continue;

		if (slots[i]->GetStack().GetID() == id)
		{
			ItemStack item = slots[i]->GetStack();
			m_slot->SetStack(item);
			m_selectedSlot = slots[i];
			return;
		}
	}
}

void UiShop::SellAll()
{
	if (m_slot->IsEmpty())
		return;

	std::string id = m_slot->GetStack().GetID();
	int amount = m_playerInventory->CountItem(id);
	m_playerInventory->RemoveItem(id, amount, false);
	int price = amount * ItemDataBase::Get()->GetItem(id)->GetSellPrice();
	GameData::Get().GetPlayerData().m_money += price;
	m_slot->Clear();
}

void UiShop::ChoosePlayerInventoryButtonClicked()
{
	Inventory* playerInventory = m_playerInventory->GetInventory();
	PlaceInventory(playerInventory);
	m_sellButton->SetIsVisible(true);
	m_sellAllButton->SetIsVisible(true);
	m_buyButton->SetIsVisible(false);
	m_shopInventory->SetVisible(false);
	m_slot->Clear();
}

void UiShop::ChooseShopInventoryButtonClicked()
{
	PlaceInventory(m_shopInventory);
	m_buyButton->SetIsVisible(true);
	m_sellButton->SetIsVisible(false);
	m_sellAllButton->SetIsVisible(false);
	m_playerInventory->GetInventory()->SetVisible(false);
	m_playerInventory->GetHotbar()->SetIsVisible(false);
	m_slot->Clear();
}

void UiShop::PlaceItemInInventoryToSlot()
{
	Inventory* inventory = nullptr;

	if (m_sellButton->GetIsVisible())
	{
		inventory = m_playerInventory->GetInventory();

		std::vector<InventorySlot*> slots = m_playerInventory->GetHotbar()->GetSlots();
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i]->IsMouseIn() && slots[i]->IsEmpty() == false)
			{
				ItemStack item = slots[i]->GetStack();
				m_slot->SetText(std::to_string(item.GetAmount()));
				m_slot->SetStack(item);
				//m_slotIndex = i;
				m_selectedSlot = slots[i];
			}
		}
	}
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
			//m_slotIndex = i;
			m_selectedSlot = slots[i];
		}
	}

}

void UiShop::PlusButton()
{
	if (m_slot->IsEmpty())
		return;

	if(m_amountOfItem >= m_playerInventory->CountItem(m_slot->GetStack().GetID()) && m_sellButton->GetIsVisible())
		return;

	int amount = m_slot->GetStack().GetAmount();
	m_slot->GetStack().SetAmount(amount + 1);
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

void UiShop::SetIsVisible(bool p_isVisible)
{
	Entity::SetIsVisible(p_isVisible);

	m_choosePlayerInventory->SetIsVisible(p_isVisible);
	m_chooseShopInventory->SetIsVisible(p_isVisible);
	m_plusButton->SetIsVisible(p_isVisible);
	m_minusButton->SetIsVisible(p_isVisible);
	m_sellButton->SetIsVisible(false);
	m_sellAllButton->SetIsVisible(false);
	m_buyButton->SetIsVisible(false);
	m_slot->SetIsVisible(p_isVisible);
	m_playerInventory->GetInventory()->SetVisible(false);
	m_shopInventory->SetVisible(false);

	//m_textBox->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("itemAmount")->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("totalPrice")->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("itemName")->SetIsVisible(p_isVisible);
}

void UiShop::Open()
{
	SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetIsVisible(false);
	ChoosePlayerInventoryButtonClicked();
}

void UiShop::Close()
{
	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{0, 300};
	SetIsVisible(false);
	m_slot->Clear();

	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
}