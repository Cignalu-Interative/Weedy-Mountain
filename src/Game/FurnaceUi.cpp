#include "pch.h"
#include "FurnaceUi.h"
#include "GameData.h"
#include "PlayerInventory.h"
#include "SpriteContainer.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"

void FurnaceUi::OnInitialize()
{
	RemoveCollider("default");
	Scene* scene = GetScene();

	AddSprite("BackgroundFurnace", "InventoryWeedyMountain", GetDimension(), 0, {}, true);
	GetSprite("BackgroundFurnace")->ResizeStretched(GetDimension());
	SyncSprites();

	AddSprite("MeltedEmpty", "emptyHorizontal", { 90,25 }, 1, {0, -165 }, true);
	AddSprite("MeltedFull", "fullHorizontalRed", { 90,25 }, 2, {0, -165 }, true);
	GetSprite("MeltedFull")->SetDimension({});

	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;

	Vector2f rowOresSlotPos = scene->GetWindowCenter() + Vector2f { -75, -225 };
	m_rawOres = scene->CreateEntity<InventorySlot>(rowOresSlotPos, { 50,50 }, "InventorySlot", Shape::Rectangle, true);
	m_rawOres->SetOnLeftClick([this] { PlaceItemInInventory(m_rawOres); });
	m_rawOres->GetCollider()->SetIsTrigger(true);
	m_rawOres->AddSprite("item_texture", "", m_rawOres->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_rawOres->SetSprite("item_texture", "");
	m_rawOres->InitText("slot" + std::to_string(0), "");

	Vector2f consumablesSlotPos = scene->GetWindowCenter() + Vector2f{ -75, -100 };
	m_consumables = scene->CreateEntity<InventorySlot>(consumablesSlotPos, { 50,50 }, "InventorySlot", Shape::Rectangle, true);
	m_consumables->SetOnLeftClick([this] { PlaceItemInInventory(m_consumables); });
	m_consumables->GetCollider()->SetIsTrigger(true);
	m_consumables->AddSprite("item_texture", "", m_consumables->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_consumables->SetSprite("item_texture", "");
	m_consumables->InitText("slot" + std::to_string(2), "");

	Vector2f meltedOresSlotPos = scene->GetWindowCenter() + Vector2f{80, -165};
	m_meltedOres = scene->CreateEntity<InventorySlot>(meltedOresSlotPos, { 50,50 }, "InventorySlot", Shape::Rectangle, true);
	m_meltedOres->SetOnLeftClick([this] {GetMeltedOres(); });
	m_meltedOres->GetCollider()->SetIsTrigger(true);
	m_meltedOres->AddSprite("item_texture", "", m_meltedOres->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_meltedOres->SetSprite("item_texture", "");
	m_meltedOres->InitText("slot" + std::to_string(1), "");

}

void FurnaceUi::OnUpdate()
{

	if (m_meltedOres->GetStack().GetAmount() == 64)
		return;

	float progress = 1.f - (m_currentHeatingTime / m_heatingTimeRequired);
	progress = Utils::Clamp(progress, 0.f, 1.f);
	int maxWidth = GetSprite("MeltedEmpty")->GetDimension().GetX();
	int height = GetSprite("MeltedEmpty")->GetDimension().GetY();
	GetSprite("MeltedFull")->SetDimension({ int(maxWidth * progress), height });


	if (m_rawOres->IsEmpty() /*|| m_consumables->IsEmpty()*/)
	{
		m_currentHeatingTime = m_heatingTimeRequired;
		return;
	}


	if (m_currentfuelTime <= 0)
	{
		if (m_consumables->IsEmpty())
			return;

		ItemStack& consumablesStack = m_consumables->GetStack();
		consumablesStack.SetAmount(consumablesStack.GetAmount() - 1);
		m_consumables->RefreshVisual();
		m_currentfuelTime = m_fuelTime;
	}

	if (m_currentfuelTime > 0)
	{
		m_currentfuelTime -= GetDeltaTime();
		m_currentHeatingTime -= GetDeltaTime();

		ItemStack& meltedStack = m_meltedOres->GetStack();

		if (m_currentHeatingTime <= 0)
		{
			ItemStack& rawStack = m_rawOres->GetStack();
			//remove 1 to m_rawOres
			rawStack.SetAmount(rawStack.GetAmount() - 1);
			m_rawOres->RefreshVisual();
			//add 1 to m_meltedOres
			{
				std::string id = m_rawOres->GetStack().GetID();
				std::string textToDelete = "_RAW";
				std::string textToAdd = "_INGOT";

				size_t pos = id.find(textToDelete);
				if (pos != std::string::npos)
					id.erase(pos, textToDelete.size());

				std::string finalId = id + textToAdd;

				//id == GOLD_INGOT
				m_meltedOres->GetStack().SetID(finalId);
				meltedStack.SetAmount(meltedStack.GetAmount() + 1);
				m_meltedOres->RefreshVisual();
			}

			m_currentHeatingTime = m_heatingTimeRequired;
		}
	}
	if (m_currentfuelTime < 0)
	{
		GetSprite("MeltedFull")->SetDimension({});

	}
}

void FurnaceUi::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		PlaceItemInRawOresSlot();
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT) )
		PlaceItemInConsumablesSlot();
}

void FurnaceUi::PlaceItemInRawOresSlot()
{
	PlaceItemInFurnaceSlot("RAW", m_rawOres);
}

void FurnaceUi::PlaceItemInConsumablesSlot()
{
	PlaceItemInFurnaceSlot("ITEM_WOOD", m_consumables);
}

void FurnaceUi::PlaceItemInFurnaceSlot(const std::string& p_idToTest, InventorySlot* p_slot)
{
	// take item with p_idToTest in inventory and place in slot
	std::vector<InventorySlot*> slots = m_playerInventory->GetHotbar()->GetSlots(); // replace by GetAllSlots
	for (InventorySlot* slot : slots)
	{
		if (slot->IsMouseIn() == false)
			continue;

		if (slot->GetStack().GetID().find(p_idToTest) == std::string::npos)
			continue;
	
		// AddItem in furnaceSlot
		{
			
			ItemData* data = ItemDataBase::Get()->GetItem(slot->GetStack().GetID());

			if (data == nullptr)
				continue;

			ItemStack& stack = slot->GetStack();

			int remaining = stack.GetAmount();

			int freeSpace = data->GetMaxStack() - p_slot->GetStack().GetAmount();
			int toAdd = std::min(freeSpace, remaining);
			p_slot->GetStack().SetAmount(p_slot->GetStack().GetAmount() + toAdd);
			p_slot->GetStack().SetID(stack.GetID());
			p_slot->RefreshVisual();

			//RemoveAmount of item toAdd in p_slot in inventory
			{

				stack.SetAmount(stack.GetAmount() - toAdd);
				slot->RefreshVisual();

				if (stack.GetAmount() == 0)
					slot->Clear();
			}
		}
	
	}
}

void FurnaceUi::GetMeltedOres()
{
	if (m_meltedOres->IsEmpty())
		return;

	std::string id = m_meltedOres->GetStack().GetID();
	int amount = m_meltedOres->GetStack().GetAmount();

	m_playerInventory->AddItem(id, amount, false);
	m_meltedOres->Clear();
}

void FurnaceUi::PlaceItemInInventory(InventorySlot* p_slot)
{
	ItemStack stack = p_slot->GetStack();
	m_playerInventory->AddItem(stack.GetID(), stack.GetAmount(), false);
	p_slot->Clear();
}

void FurnaceUi::Open()
{
	Vector2f pos = GetPosition() + Vector2f{0,50};

	m_playerInventory->GetInventory()->SetPosition(pos);
	m_playerInventory->GetInventory()->SetVisible(true);

	m_playerInventory->GetHotbar()->SetPosition(pos + Vector2f {0, 150});
	m_playerInventory->GetHotbar()->SetIsVisible(true);

	SetIsVisible(true);
	m_rawOres->SetIsVisible(true);
	m_meltedOres->SetIsVisible(true);
	m_consumables->SetIsVisible(true);

	m_rawOres->RefreshVisual();
	m_meltedOres->RefreshVisual();
	m_consumables->RefreshVisual();
}

void FurnaceUi::Close()
{
	Vector2f hotbarPos = GetScene()->GetWindowCenter() + Vector2f{ 0, 300 };
	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
	m_playerInventory->GetInventory()->SetVisible(false);

	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos);

	SetIsVisible(false);
	m_rawOres->SetIsVisible(false);
	m_meltedOres->SetIsVisible(false);
	m_consumables->SetIsVisible(false);

	m_rawOres->RefreshVisual();
	m_meltedOres->RefreshVisual();
	m_consumables->RefreshVisual();
}