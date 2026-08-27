#include "pch.h"
#include "FurnaceUi.h"
#include "GameData.h"
#include "PlayerInventory.h"
#include "SpriteContainer.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "Animation.h"

void FurnaceUi::OnInitialize()
{
	RemoveCollider("default");
	Scene* scene = GetScene();

	AddSprite("BackgroundFurnace", "Furnace_BackGround", GetDimension(), 0, {}, true);
	GetSprite("BackgroundFurnace")->ResizeStretched(GetDimension());
	SyncSprites();

	AddSprite("EmptyProgressBar", "EmptyProgressBar", { 100,25 }, 1, { 0, -165 }, true);
	GetSprite("EmptyProgressBar")->SetIsVisible(true);

	AddSprite("EmptyFlame", "EmptyFlame", { 32,32 }, 1, { -75, -165 }, true);
	GetSprite("EmptyFlame")->SetIsVisible(true);
	/*AddSprite("MeltedEmpty", "emptyHorizontal", { 90,25 }, 1, {0, -165 }, true);
	AddSprite("MeltedFull", "fullHorizontalRed", { 90,25 }, 2, {0, -165 }, true);
	GetSprite("MeltedFull")->SetDimension({});*/

	m_playerInventory = GameData::Get().GetPlayerData().m_playerInventory;

	Vector2f rowOresSlotPos = scene->GetWindowCenter() + Vector2f { -75, -225 };
	m_rawOres = scene->CreateEntity<InventorySlot>(rowOresSlotPos, { 50,50 }, "Slot_Furnace", Shape::Rectangle, true);
	m_rawOres->SetOnLeftClick([this] { PlaceItemInInventory(m_rawOres); });
	m_rawOres->GetCollider()->SetIsTrigger(true);
	m_rawOres->AddSprite("item_texture", "", m_rawOres->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_rawOres->SetSprite("item_texture", "");
	m_rawOres->InitText("slot" + std::to_string(0), "");

	Vector2f consumablesSlotPos = scene->GetWindowCenter() + Vector2f{ -75, -100 };
	m_consumables = scene->CreateEntity<InventorySlot>(consumablesSlotPos, { 50,50 }, "Slot_Furnace", Shape::Rectangle, true);
	m_consumables->SetOnLeftClick([this] { PlaceItemInInventory(m_consumables); });
	m_consumables->GetCollider()->SetIsTrigger(true);
	m_consumables->AddSprite("item_texture", "", m_consumables->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_consumables->SetSprite("item_texture", "");
	m_consumables->InitText("slot" + std::to_string(2), "");

	Vector2f meltedOresSlotPos = scene->GetWindowCenter() + Vector2f{80, -165};
	m_meltedOres = scene->CreateEntity<InventorySlot>(meltedOresSlotPos, { 50,50 }, "Slot_Furnace", Shape::Rectangle, true);
	m_meltedOres->SetOnLeftClick([this] {GetMeltedOres(); });
	m_meltedOres->GetCollider()->SetIsTrigger(true);
	m_meltedOres->AddSprite("item_texture", "", m_meltedOres->GetDimension(), 1, { 0.0f, 0.0f }, true);
	m_meltedOres->SetSprite("item_texture", "");
	m_meltedOres->InitText("slot" + std::to_string(1), "");

	SpriteContainer* progressBar = AddSprite("ProgressBarSpriteSheet", "Furnace_progresBar", {100,25}, 1, {0, -165}, true);
	GetSprite("ProgressBarSpriteSheet")->SetIsVisible(false);
	m_progressBarAnimation = new Animation();
	m_progressBarAnimation->Init(progressBar->m_sprite, "default", true, "../../res/Game/progressBarAnimation.json");

	SpriteContainer* flame = AddSprite("FlameSpriteSheet", "Furnace_Fuel", { 32,32 }, 1, { -75, -165 }, true);
	GetSprite("FlameSpriteSheet")->SetIsVisible(false);
	m_flameAnimation = new Animation();
	m_flameAnimation->Init(flame->m_sprite, "default", true, "../../res/Game/fuelAnimation.json");
	m_flameAnimation->SetDuration(m_fuelTime / m_flameAnimation->GetTotalFrames());
	
}

void FurnaceUi::OnUpdate()
{	

	float dt = GetDeltaTime();

	if (m_currentfuelTime > 0)
	{
		m_currentfuelTime -= dt;
		m_flameAnimation->Update(dt);
		GetSprite("FlameSpriteSheet")->SetIsVisible(true);
		GetSprite("EmptyFlame")->SetIsVisible(false);
	}
	else
	{
		// hide spriteSheet and reset animation
		GetSprite("FlameSpriteSheet")->SetIsVisible(false);
		GetSprite("EmptyFlame")->SetIsVisible(true);
		m_flameAnimation->ResetAnimation();
	}

	if (m_rawOres->IsEmpty())
	{
		GetSprite("EmptyProgressBar")->SetIsVisible(true);
		m_progressBarAnimation->ResetAnimation();
		//GetSprite("ProgressBarSpriteSheet")->SetIsVisible(false);
		m_currentHeatingTime = m_heatingTimeRequired;
		return;
	}

	GetSprite("EmptyProgressBar")->SetIsVisible(false);
	GetSprite("ProgressBarSpriteSheet")->SetIsVisible(true);

	ItemData* data = ItemDataBase::Get()->GetItem(m_rawOres->GetStack().GetID());
	if (data == nullptr)
	{
		return;
	}

	std::string resultItem = data->GetCookResultItemID();

	if (m_meltedOres->IsEmpty() == false)
	{
		if (m_meltedOres->GetStack().GetID() != resultItem)
			return;
	}

	if (m_meltedOres->GetStack().GetAmount() == 64)
		return;

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
		m_currentHeatingTime -= dt;
		m_progressBarAnimation->Update(dt);
		

		ItemStack& meltedStack = m_meltedOres->GetStack();

		if (m_currentHeatingTime <= 0)
		{
	
			int heatingTime = data->GetCookTime();

			ItemStack& rawStack = m_rawOres->GetStack();
			//remove 1 to m_rawOres
			rawStack.SetAmount(rawStack.GetAmount() - 1);
			m_rawOres->RefreshVisual();
			//add 1 to m_meltedOres
			{

				m_meltedOres->GetStack().SetID(resultItem);
				meltedStack.SetAmount(meltedStack.GetAmount() + 1);
				m_meltedOres->RefreshVisual();
			}
			m_heatingTimeRequired = heatingTime;
			m_currentHeatingTime = heatingTime;
		}
	}
}

void FurnaceUi::OnEvent()
{
	if(m_isVisible == false)
		return;

	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		PlaceItemInRawOresSlot();
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT) )
		PlaceItemInConsumablesSlot();
}

void FurnaceUi::PlaceItemInRawOresSlot()
{
	PlaceItemInFurnaceSlot(m_rawOres);
}

void FurnaceUi::PlaceItemInConsumablesSlot()
{
	PlaceItemInFurnaceSlot("ITEM_WOOD", m_consumables);
}

void FurnaceUi::PlaceItemInFurnaceSlot( InventorySlot* p_slot)
{
	if (m_isVisible == false)
		return;
	// take item with p_idToTest in inventory and place in slot
	std::vector<InventorySlot*> slots = m_playerInventory->GetAllSlots(); // replace by GetAllSlots
	for (InventorySlot* slot : slots)
	{
		if (slot->IsMouseIn() == false)
			continue;
		ItemData* data = ItemDataBase::Get()->GetItem(slot->GetStack().GetID());

		if (data == nullptr)
			continue;
		
		if (data->IsCookable() == false)
			return;

		m_currentHeatingTime = data->GetCookTime();
		m_heatingTimeRequired = data->GetCookTime();
		m_progressBarAnimation->SetAnimationByName("default");
		m_progressBarAnimation->SetDuration(data->GetCookTime()/ m_progressBarAnimation->GetTotalFrames());

		// AddItem in furnaceSlot
		{

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

void FurnaceUi::PlaceItemInFurnaceSlot(const std::string& p_idToTest, InventorySlot* p_slot)
{
	if (m_isVisible == false)
		return;
	// take item with p_idToTest in inventory and place in slot
	std::vector<InventorySlot*> slots = m_playerInventory->GetAllSlots(); // replace by GetAllSlots
	for (InventorySlot* slot : slots)
	{
		if (slot->IsMouseIn() == false)
			continue;
		ItemData* data = ItemDataBase::Get()->GetItem(slot->GetStack().GetID());

		if (data == nullptr)
			continue;

		if (slot->GetStack().GetID().find(p_idToTest) == std::string::npos)
			continue;
	
		// AddItem in furnaceSlot
		{

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
	SetPosition(GetScene()->GetWindowCenter());

	m_playerInventory->GetInventory()->SetVisible(true);

	m_playerInventory->GetHotbar()->SetIsVisible(true);

	SetIsVisible(true);
	m_rawOres->SetIsVisible(true);
	m_meltedOres->SetIsVisible(true);
	m_consumables->SetIsVisible(true);

	m_rawOres->RefreshVisual();
	m_meltedOres->RefreshVisual();
	m_consumables->RefreshVisual();

	m_isOpen = true;
}

void FurnaceUi::Close()
{
	Vector2f hotbarPos = { GetScene()->GetWindowCenter().GetX(),(float)GetScene()->GetWindowSize().GetY() - 12 };

	m_playerInventory->GetInventory()->SetPosition(GetScene()->GetWindowCenter());
	m_playerInventory->GetInventory()->SetVisible(false);

	m_playerInventory->GetHotbar()->SetIsVisible(true);
	m_playerInventory->GetHotbar()->SetPosition(hotbarPos, { 0.5f, 1.0f });

	SetIsVisible(false);
	m_rawOres->SetIsVisible(false);
	m_meltedOres->SetIsVisible(false);
	m_consumables->SetIsVisible(false);

	m_rawOres->RefreshVisual();
	m_meltedOres->RefreshVisual();
	m_consumables->RefreshVisual();

	m_isOpen = false;
}

void FurnaceUi::Save(nlohmann::json& out) const
{
	out["rawOres"] = { {"id", m_rawOres->GetStack().GetID()}, {"amount", m_rawOres->GetStack().GetAmount()} };
	out["meltedOres"] = { {"id", m_meltedOres->GetStack().GetID()}, {"amount", m_meltedOres->GetStack().GetAmount()} };
	out["consumables"] = { {"id", m_consumables->GetStack().GetID()}, {"amount", m_consumables->GetStack().GetAmount()} };

	out["currentFuelTime"] = m_currentfuelTime;
	out["currentHeatingTime"] = m_currentHeatingTime;
	out["heatingTimeRequired"] = m_heatingTimeRequired;
}

void FurnaceUi::Load(const nlohmann::json& in)
{
	auto loadSlot = [](InventorySlot* p_slot, const nlohmann::json& p_data)
		{
			std::string id = p_data.value("id", "ITEM_NOTHING");
			int amount = p_data.value("amount", 0);

			if (id.empty() || id == "ITEM_NOTHING" || amount <= 0)
				p_slot->Clear();
			else
				p_slot->SetStack(ItemStack(id, amount));

			p_slot->RefreshVisual();
		};

	if (in.contains("rawOres"))     
		loadSlot(m_rawOres, in["rawOres"]);

	if (in.contains("meltedOres")) 
		loadSlot(m_meltedOres, in["meltedOres"]);

	if (in.contains("consumables"))
		loadSlot(m_consumables, in["consumables"]);

	m_currentfuelTime = in.value("currentFuelTime", 0.f);
	m_currentHeatingTime = in.value("currentHeatingTime", m_heatingTimeRequired);
	m_heatingTimeRequired = in.value("heatingTimeRequired", 0.f);

	if (m_rawOres->IsEmpty() == false && m_heatingTimeRequired > 0.f)
	{
		m_progressBarAnimation->SetDuration(m_heatingTimeRequired / m_progressBarAnimation->GetTotalFrames());
	}
}

void FurnaceUi::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);

	Vector2f pos = GetPosition() + Vector2f{ 0,50 };
	m_playerInventory->GetInventory()->SetPosition(pos);

	m_playerInventory->GetHotbar()->SetPosition(pos + Vector2f{ 0, 150 });

	Scene* scene = GetScene();

	Vector2f rowOresSlotPos = scene->GetWindowCenter() + Vector2f{ -75, -225 };
	m_rawOres->SetPosition(rowOresSlotPos);

	Vector2f consumablesSlotPos = scene->GetWindowCenter() + Vector2f{ -75, -100 };
	m_consumables->SetPosition(consumablesSlotPos);

	Vector2f meltedOresSlotPos = scene->GetWindowCenter() + Vector2f{ 80, -165 };
	m_meltedOres->SetPosition(meltedOresSlotPos);
}