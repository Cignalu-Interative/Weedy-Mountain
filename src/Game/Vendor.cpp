#include "pch.h"
#include "Vendor.h"

void Vendor::OnInitialize()
{
	Scene* scene = GetScene();

	Vector2f topRightPos = GetPosition({ 1,0 });

	m_slot = scene->CreateEntity<InventorySlot>({ topRightPos - Vector2f{200.f, 100.f} }, { 100,50 }, "", Shape::Rectangle, true);

	float offsetButtonX = GetPosition({ 1,1 }).GetX() - 50.f;
	float offsetButtonY = GetPosition({ 1,1 }).GetY() - 100.f;

	m_sellButton = scene->CreateButton({ offsetButtonX,GetPosition({ 1,1 }).GetY() }, { 100,50 }, "", "sellButton", "SELL");
	m_sellButton->SetOnLeftClick([] {std::cout << "SELL ITEM" << std::endl; });

	m_buyButton = GetScene()->CreateButton({ GetPosition({ 1,1 }).GetX(),offsetButtonY }, { 100,50 }, "", "buyButton", "BUY");
	m_buyButton->SetOnLeftClick([] {std::cout << "BUY ITEM" << std::endl; });


	float offsetPriceX = GetPosition({ 0.5,1 }).GetX() - 200;
	float offsetPriceY = GetPosition({ 0.5,1 }).GetY();

	Vector2f posItemAmountText = m_slot->GetPosition() - Vector2f{ 0, m_slot->GetDimension().GetY() / 2 - 300 };
	float amountOfItem = m_slot->GetStack().GetAmount();

	scene->CreateText("totalPrice", { offsetPriceX,offsetPriceX }, std::to_string(amountOfItem /*item price*/), ColorsNamed::White);
	scene->CreateText("itemAmount", { offsetPriceX,offsetPriceX }, std::to_string(amountOfItem ), ColorsNamed::White);
}
