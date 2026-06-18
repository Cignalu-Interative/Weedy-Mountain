#include "pch.h"
#include "UiShop.h"

void UiShop::OnInitialize()
{
	Scene* scene = GetScene();

	//Vector2f topRightPos = GetPosition({ 1,0 });

	Vector2f pos = GetPosition();

	m_slot = scene->CreateEntity<InventorySlot>(pos, {100,50}, "Oak_Tree", Shape::Rectangle);

	////m_inventory = scene->CreateEntity<Inventory>({ topRightPos - Vector2f{200.f, 100.f} }, { 100,50 }, "", Shape::Rectangle, true);
	//m_inventory = scene->CreateEntity<Inventory>(GetPosition(), { 636, 306 }, "InventoryWeedyMountain", Shape::Rectangle, true);
	//m_inventory->GetCollider()->SetIsTrigger(true);

	//std::vector<InventorySlot*> slots = m_inventory->GetSlots();
	//for (InventorySlot* slot : slots)
	//{
	//	slot->GetCollider()->SetIsTrigger(true);
	//	std::cout << slot->GetCollider()->GetDimension().GetX() << std::endl;
	//}
	//m_inventory->SetVisible(false);


	float offsetButtonX = GetPosition({ 1,1 }).GetX() - 50.f;
	float offsetButtonY = GetPosition({ 1,1 }).GetY() - 100.f;

	m_sellButton = scene->CreateButton({scene->GetWindowWidth() /2.f, scene->GetWindowHeight()/2.f}, {50,25}, "Oak_Tree", "sellButton", "SELL", true);
	m_sellButton->SetOnLeftClick([] {std::cout << "SELL ITEM" << std::endl; });
	m_sellButton->ShowCollider(true);

	m_buyButton = scene->CreateButton({ scene->GetWindowWidth() / 2.5f, scene->GetWindowHeight() / 2.f  }, { 50,25 }, "Oak_Tree", "buyButton", "BUY", true);
	m_buyButton->SetOnLeftClick([] {std::cout << "BUY ITEM" << std::endl; });
	m_buyButton->ShowCollider(true);



	//float offsetPriceX = GetPosition({ 0.5,1 }).GetX() - 200;
	//float offsetPriceY = GetPosition({ 0.5,1 }).GetY();

	//Vector2f posItemAmountText = m_inventory->GetPosition() - Vector2f{ 0, m_inventory->GetDimension().GetY() / 2 - 300 };
	//float amountOfItem = m_inventory->GetSlots()

	//scene->CreateText("totalPrice", { offsetPriceX,offsetPriceX }, std::to_string(amountOfItem /*item price*/), ColorsNamed::White);
	//scene->CreateText("itemAmount", { offsetPriceX,offsetPriceX }, std::to_string(amountOfItem ), ColorsNamed::White);
}
