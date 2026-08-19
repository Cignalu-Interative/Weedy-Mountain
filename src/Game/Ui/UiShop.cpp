#include "pch.h"

#include "UiShop.h"
#include "Player.h"
#include "GameScene.h"
#include "2DPendentLib/InputManager.h"
#include "ItemDataBase.h"
#include "SpriteContainer.h"
#include "TextBox.h"

#include "PlayerInventory.h"

#include "QuestManager.h"

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
	m_slot->ShowCollider(true);

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

	m_choosePlayerInventory = scene->CreateButton({ offsetPlayerInventory }, { 170,50 }, "button", "player", "Player Inventory");
	m_choosePlayerInventory->GetSprite()->ResizeStretched(m_choosePlayerInventory->GetDimension());
	m_choosePlayerInventory->SyncSprites();
	m_choosePlayerInventory->SetOnLeftClick([this] {ChoosePlayerInventoryButtonClicked(); });
	m_choosePlayerInventory->SetOnHoverStay([this] {m_choosePlayerInventory->SetSprite("button2"); });
	m_choosePlayerInventory->SetOnHoverExit([this] {m_choosePlayerInventory->SetSprite("button"); });
	m_choosePlayerInventory->GetCollider()->SetIsTrigger(true);
	m_choosePlayerInventory->ShowCollider(true);
	m_choosePlayerInventory->GetCollider()->SetMask(0);

	m_chooseShopInventory = scene->CreateButton({ offsetShopInventory }, { 170,50 }, "button", "shop", "Shop Inventory");
	m_chooseShopInventory->GetSprite()->ResizeStretched(m_choosePlayerInventory->GetDimension());
	m_chooseShopInventory->SyncSprites();
	m_chooseShopInventory->SetOnLeftClick([this] {ChooseShopInventoryButtonClicked(); });
	m_chooseShopInventory->SetOnHoverStay([this] {m_chooseShopInventory->SetSprite("button2"); });
	m_chooseShopInventory->SetOnHoverExit([this] {m_chooseShopInventory->SetSprite("button"); });
	m_chooseShopInventory->GetCollider()->SetIsTrigger(true);
	m_chooseShopInventory->GetCollider()->SetMask(0);

	Vector2f halfRightPos = GetPosition({ 1,0.5f });
	float offsetPriceX = GetPosition().GetX();
	float offsetPriceY = halfRightPos.GetY() * 1.275f;
	float offsetItemNameY = halfRightPos.GetY() * 0.84f;
	Vector2f posItemName = m_slot->GetPosition() + Vector2f{0, 80};
	//m_textBox = scene->CreateTextBox("itemAmount", { offset.GetX(),halfRightPos.GetY() }, { 50,25 }, ColorsNamed::White);
	//scene->CreateText("itemAmount", { offset.GetX(),halfRightPos.GetY() }, "Item : " + std::to_string(m_amountOfItem), ColorsNamed::White, true);
	scene->CreateText("totalPrice", { offset.GetX(),offsetPriceY }, "Price : " + std::to_string(m_totalPrices), ColorsNamed::White, true);
	scene->CreateText("itemName", {}, "", ColorsNamed::White, true);

	Vector2f plusButtonOffset = { GetPosition({ 1,0.5f }).GetX() - GetDimension().GetX() / 8.f, GetPosition({ 1,0.5f }).GetY() };
	Vector2f minusButtonOffset = { GetPosition({ 1,0.5f }).GetX() - GetDimension().GetX() / 3.62f, GetPosition({ 1,0.5f }).GetY() };

	m_plusButton = scene->CreateButton(plusButtonOffset, { 40,40 }, "button", "plusButton", "+");
	m_plusButton->GetSprite()->ResizeStretched(m_plusButton->GetDimension());
	m_plusButton->SyncSprites();
	m_plusButton->SetOnLeftClick([this] {PlusButton(); });
	m_plusButton->SetOnHoverStay([this] {m_plusButton->SetSprite("button2"); });
	m_plusButton->SetOnHoverExit([this] {m_plusButton->SetSprite("button"); });
	m_plusButton->GetCollider()->SetIsTrigger(true);
	m_plusButton->GetCollider()->SetMask(0);
	m_plusButton->ShowCollider(true);

	m_minusButton = scene->CreateButton(minusButtonOffset, { 40,40 }, "button", "minusButton", "-");
	m_minusButton->GetSprite()->ResizeStretched(m_minusButton->GetDimension());
	m_minusButton->SyncSprites();
	m_minusButton->SetOnLeftClick([this] {MinusButton(); });
	m_minusButton->SetOnHoverStay([this] {m_minusButton->SetSprite("button2"); });
	m_minusButton->SetOnHoverExit([this] {m_minusButton->SetSprite("button"); });
	m_minusButton->GetCollider()->SetIsTrigger(true);
	m_minusButton->GetCollider()->SetMask(0);

	Vector2f textBoxPos = { offset.GetX(),halfRightPos.GetY() };//GetPosition() - Vector2f{ 200,180 };
	m_textBox = scene->CreateTextBox("amountUiShop", textBoxPos, { 110, 40 }, ColorsNamed::Transparent, true);
	m_textBox->GetCollider()->SetIsTrigger(true);
	m_textBox->SetDrawLayer(INT_MAX);
	m_textBox->GetText()->SetColor(ColorsNamed::White);
	m_textBox->SetIsVisible(false);
	
	SetDrawLayer(GetDrawLayer() - 20);

}

void UiShop::OnEvent()
{
	if (m_isVisible == false)
		return;

	InputManager* im = InputManager::Get();

	if (im->IsMouseDown(BUTTON_LEFT))
		PlaceItemInInventoryToSlot();

	if (im->IsMouseDown(BUTTON_LEFT) && m_slot->IsMouseIn())
		m_slot->Clear();

	if (im->IsMouseDown(BUTTON_LEFT) && m_textBox->IsMouseIn())
	{
		im->StartTextInput();
	}
	if (im->IsMouseDown(BUTTON_LEFT) && !m_textBox->IsMouseIn())
	{
		im->StopTextInput();
	}
	if (im->IsTextInputActive() == true && im->IsKeyboardDown(K_BACKSPACE))
	{
		im->DeleteLastCharInFinalTextInput();
	}
	if (im->IsTextInputActive() == true)
	{
		m_searchText = im->GetfinalTextInput();
		m_textBox->SetTextInBox(m_searchText);
	}
	if (im->IsKeyboardDown(K_ESCAPE))
	{
		Close();
	}
}

void UiShop::OnUpdate()
{
	if (m_isVisible == false)
		return;

	bool r = false;
	if (m_searchText.empty() == false)
	{
		m_amountOfItem = std::stoi(m_searchText);
		r = true;
	}

	int price = 0;
	if(r == false)
		m_amountOfItem = m_slot->GetStack().GetAmount();

	if (m_sellButton->GetIsVisible() && m_slot->IsEmpty() == false)
		price = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetSellPrice();

	if (m_buyButton->GetIsVisible() && m_slot->IsEmpty() == false)
		price = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetBuyPrice();

	m_totalPrices = m_amountOfItem/*m_slot->GetStack().GetAmount()*/ * price;

	m_textBox->SetTextInBox(std::to_string(m_amountOfItem));
	//GetScene()->GetGeometry<Text>("itemAmount")->SetText("Item : " + std::to_string(m_amountOfItem));
	GetScene()->GetGeometry<Text>("totalPrice")->SetText("Price : " + std::to_string(m_totalPrices));
	if (m_slot->IsEmpty())
	{
		GetScene()->GetGeometry<Text>("itemName")->SetText("");
		return;
	}
	std::string itemName = ItemDataBase::Get()->GetItem(m_slot->GetStack().GetID())->GetName();
	Vector2f posItemName = m_slot->GetPosition() + Vector2f{ 0, 85 };
	GetScene()->GetGeometry<Text>("itemName")->SetPosition(posItemName);
	GetScene()->GetGeometry<Text>("itemName")->SetText(itemName);

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
	int amount = m_amountOfItem; // m_slot->GetStack().GetAmount();

	if(m_playerInventory->HasItems(id,amount))
	{
		//Add 15% at the total price if it s different season
		//if (GameData::Get().GetSeason() != /*Get season of item*/)
		//	m_totalPrices * 1, 15;

		GameData::Get().GetPlayerData().m_money += m_totalPrices;
		m_playerInventory->RemoveItem(id, amount, false);
		m_slot->Clear();

		QuestManager::Get()->NotifyEvent(QuestEvent(QuestEventType::ItemSold, id, amount));
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

	//Add 15% at the total price if it s different season
	//if (GameData::Get().GetSeason() != /*Get season of item*/)
	//	price * 1, 15;

	GameData::Get().GetPlayerData().m_money += price;
	m_slot->Clear();

	if (amount > 0)
		QuestManager::Get()->NotifyEvent(QuestEvent(QuestEventType::ItemSold, id, amount));
}

void UiShop::ChoosePlayerInventoryButtonClicked()
{
	Inventory* playerInventory = m_playerInventory->GetInventory();
	PlaceInventory(playerInventory);
	m_sellButton->SetIsVisible(true);
	m_sellAllButton->SetIsVisible(true);
	m_buyButton->SetIsVisible(false);
	if (m_shopInventory)
		m_shopInventory->SetVisible(false);
	m_slot->Clear();
}

void UiShop::ChooseShopInventoryButtonClicked()
{
	if (m_shopInventory)
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

	if (inventory == nullptr)
		return;
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
	m_textBox->SetIsVisible(p_isVisible);

	if(m_shopInventory)
		m_shopInventory->SetVisible(false);

	//m_textBox->SetIsVisible(p_isVisible);
	//GetScene()->GetGeometry<Text>("itemAmount")->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("totalPrice")->SetIsVisible(p_isVisible);
	GetScene()->GetGeometry<Text>("itemName")->SetIsVisible(p_isVisible);
}

void UiShop::Open()
{
	SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetIsVisible(false);
	ChoosePlayerInventoryButtonClicked();
	m_playerInventory->LockEvent();
}

void UiShop::Close()
{
	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{0, 300};
	m_slot->Clear();
	m_textBox->SetIsVisible(false);
	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());

	m_playerInventory->UnLockEvent();
}