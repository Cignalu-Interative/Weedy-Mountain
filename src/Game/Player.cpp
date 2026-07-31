#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

#include "FarmManager.h"   
#include "GameData.h" 
#include "Inventory.h"
#include "PlayerInventory.h"

#include "PlayerState.h"
#include "PlayerConditions.h"

#include "Aleatory.h"

#include "EnviroProps.h"
#include "ItemDataBase.h"
#include "ItemActionRegistry.h"

#include "FishLootTable.h"

void Player::OnInitialize()
{

	//gm.SetInventory(PlayerInventory(inv, hot));

	PlayerJob gatherer;
	gatherer.m_name = JobType::Gatherer;
	gm.GetPlayerData().m_jobs.push_back(gatherer);

	PlayerJob farmer;
	farmer.m_name = JobType::Farmer;
	gm.GetPlayerData().m_jobs.push_back(farmer);

	PlayerJob miner;
	miner.m_name = JobType::Miner;
	gm.GetPlayerData().m_jobs.push_back(miner);

	/*PlayerJob* killer = new PlayerJob();
	killer->m_name = JobType::Killer;
	m_jobs.push_back(killer);
	PlayerJob* dealer = new PlayerJob();
	dealer->m_name = JobType::Dealer;
	m_jobs.push_back(dealer);*/

	m_fishingZoneCollider = AddCollider("fishingZone", { 8, 8 }, Shape::Rectangle, { 0.f, 0.f }, true);

	SetupStateMachine();

}

void Player::OnEvent()
{
	if (m_isVisible == false)
		return;

	if (InputManager::Get()->IsKeyboardDown(K_KP_PLUS))
		GameData::Get().GetPlayerData().m_playerInventory->AddItem("ITEM_WOOD", 10, false);

	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(gm.GetPlayerData().m_stats.m_playerSpeed * 2);
	}
	else SetSpeed(gm.GetPlayerData().m_stats.m_playerSpeed);

	Vector2f direction = GetDirection();
	float currentSpeed = InputManager::Get()->IsKeyboardHeld(K_LSHIFT) ? gm.GetPlayerData().m_stats.m_playerSpeed * 2.f : gm.GetPlayerData().m_stats.m_playerSpeed;
	SetSpeed(currentSpeed);

	/*if (InputManager::Get()->IsKeyboardDown(K_TAB))
	{
		LockInputs();
	}*/

	Vector2f dir = {0.f, 0.f};
	Collider* boxCollider = GetCollider("actionBox");
	//Input Clavier
	// Mettre les inputs pour lesquels on ne veux pas pouvoir les detecter lorsqu'on est dans un shop dialogues etc ... 
	if (m_inputBlocker == false)
	{
		

		//Input Clavier
		if (InputManager::Get()->IsKeyboardHeld(K_Z))
		{
			dir.SetY(dir.GetY() - 1.f);
			boxCollider->SetOffset({ 0, -8 });
			m_facingDirection = { 0.f, -1.f };
		}
		if (InputManager::Get()->IsKeyboardHeld(K_S)) 
		{
			dir.SetY(dir.GetY() + 1.f);
			boxCollider->SetOffset({ 0, 24 });
			m_facingDirection = { 0.f, 1.f };
		}
		if (InputManager::Get()->IsKeyboardHeld(K_Q)) 
		{
			dir.SetX(dir.GetX() - 1.f);
			boxCollider->SetOffset({ -16, 8 });
			m_facingDirection = { -1.f, 0.f };
		}
		if (InputManager::Get()->IsKeyboardHeld(K_D))
		{
			dir.SetX(dir.GetX() + 1.f);
			boxCollider->SetOffset({ 16, 8 });
			m_facingDirection = { 1.f, 0.f };
		}

		SetDirection(dir);

		if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
			HandlePrimaryAction();
	}

	float len = std::sqrtf(dir.GetX() * dir.GetX() + dir.GetY() * dir.GetY());

	m_isMovingThisFrame = (len > 0.f);

	if (len > 0.f)
		dir = { dir.GetX() / len, dir.GetY() / len };

	PhysicsBody* body = GetPhysicsBody();
	if (body != nullptr)
	{
		body->SetLinearVelocity({dir.GetX() * currentSpeed, dir.GetY() * currentSpeed});
	}
}

void Player::HandlePrimaryAction()
{
	PlayerInventory* inv = GetInventory();

	if (inv != nullptr)
	{
		const std::string& selectedID = inv->GetSelectedItemID();

		if (selectedID != "ITEM_NOTHING")
		{
			ItemData* data = ItemDataBase::Get()->GetItem(selectedID);

			if (data != nullptr && data->GetUseActionID().empty() == false)
			{
				bool executed = ItemActionRegistry::Get()->Execute(data->GetUseActionID(), *this, *inv, selectedID, inv->GetSelectedIndex());

				if (executed)
					return;
			}
		}
	}

	HandleDefaultHarvestAction();
}

void Player::HandleDefaultHarvestAction()
{
	Collider* actionBox = GetCollider("actionBox");

	if (actionBox == nullptr)
		return;

	FarmTile* farmTarget = FarmManager::Get()->GetTileInActionBox(actionBox);

	if (farmTarget != nullptr && farmTarget->GetState() == FarmTileState::IDLE)
	{
		PlayerInventory* inv = GetInventory();
		if (inv == nullptr)
			return;

		const std::string& selectedID = inv->GetSelectedItemID();
		ItemData* data = (selectedID != "ITEM_NOTHING") ? ItemDataBase::Get()->GetItem(selectedID) : nullptr;

		bool hasSpadeInHand = (data != nullptr && data->GetItemType() == ItemType::Tool && data->GetToolType() == ToolType::Spade);

		if (hasSpadeInHand == false)
			return;
	}

	HandleFarmAction();
}

void Player::TryHitEnviroProps(EnviroProps* props)
{
	if (props == nullptr)
		return;

	PlayerInventory* inv = GetInventory();
	if (inv == nullptr)
		return;

	const std::string& selectedID = inv->GetSelectedItemID();
	ItemData* data = (selectedID != "ITEM_NOTHING") ? ItemDataBase::Get()->GetItem(selectedID) : nullptr;

	if (data != nullptr && data->GetUseActionID().empty() == false)
		return;

	if (data == nullptr || data->GetItemType() != ItemType::Tool)
		return;

	props->TryHit(data->GetToolType(), data->GetToolRarity());
}

void Player::HandleFarmAction()
{
	Collider* actionBox = GetCollider("actionBox");

	if (!actionBox)
		return;

	FarmTile* target = FarmManager::Get()->GetTileInActionBox(actionBox);

	if (!target)
		return;

	int currentDay = GameData::Get().GetDay();

	switch (target->GetState())
	{
	case FarmTileState::IDLE:
		target->Till(currentDay);
		break;

	case FarmTileState::READY:
		target->Harvest(currentDay);
		break;

	default:
		break;
	}
}

void Player::StartFishing()
{
	if (m_fishingState != FishingState::Idle)
		return;

	if (m_fishingZoneCollider == nullptr)
		return;

	m_fishingZoneCollider->SetOffset({ m_facingDirection.GetX() * FISHING_CAST_DISTANCE, m_facingDirection.GetY() * FISHING_CAST_DISTANCE });

	m_fishingZoneTouchingWater = false;
	m_castSyncTimer = 0.f;
	m_fishingState = FishingState::Casting;
}

void Player::RetractFishingRod()
{
	if (m_fishingZoneCollider != nullptr)
		m_fishingZoneCollider->SetOffset({ 0.f, 0.f });

	GetScene()->RemoveGeometry("debug_fishing");

	m_fishingZoneTouchingWater = false;
	m_castSyncTimer = 0.f;
	m_fishingState = FishingState::Idle;
}

void Player::UpdateFishing()
{
	float dt = GetDeltaTime();

	switch (m_fishingState)
	{
	case FishingState::Idle:
		break;

	case FishingState::Casting:
	{
		m_castSyncTimer += dt;

		if (m_castSyncTimer < FISHING_CAST_SYNC_DELAY)
			break;

		if (m_fishingZoneTouchingWater)
		{
			m_fishingState = FishingState::Waiting;
			m_fishingTimer = 0.f;
			m_fishingTimerTarget = Aleatory::GetFloat(2.f, 10.f);
		}
		else
		{
			std::cout << "[Fishing] l'hamecon n'est pas dans l'eau, cane range !" << std::endl;
			RetractFishingRod();
		}
		break;
	}

	case FishingState::Waiting:
	{
		if (m_isMovingThisFrame)
		{
			std::cout << "[Fishing] mouvement detecter, cane range !" << std::endl;
			RetractFishingRod();
			break;
		}
		if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		{
			std::cout << "[Fishing] cane retracte !" << std::endl;
			RetractFishingRod();
			break;
		}

		m_fishingTimer += dt;
		if (m_fishingTimer >= m_fishingTimerTarget)
		{
			Vector2f zonePos = m_fishingZoneCollider->GetPosition({ 0.5f, 0.5f });
			GetScene()->CreateFillRect("debug_fishing", zonePos, { 16, 16 }, Color(255, 0, 0, 255));

			m_biteReactionTimer = 0.f;
			m_fishingState = FishingState::BiteWindow;
		}
		break;
	}

	case FishingState::BiteWindow:
	{
		if (m_isMovingThisFrame)
		{
			std::cout << "[Fishing] mouvement detecter, cane range !" << std::endl;
			RetractFishingRod();
			break;
		}

		if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		{
			std::string fish = FishLootTable::PickOne();

			std::cout << "[Fishing] poisson attraper : " << fish << std::endl;

			GetInventory()->AddItem(fish, 1, false);
			RetractFishingRod();
			break;
		}

		m_biteReactionTimer += dt;
		if (m_biteReactionTimer >= FISHING_BITE_WINDOW)
		{
			std::cout << "[Fishing] trop lent !" << std::endl;
			RetractFishingRod();
		}
		break;
	}
	}
}

void Player::OnUpdate()
{
	m_stateMachine.Update(this, GetDeltaTime());
	SetDrawLayer(200 + GetCollider()->GetPosition({ 1.f,1.f }).GetY());
	//std::cout << GetPosition().GetX() << "   " << GetPosition().GetY() << std::endl;
	UpdateJobs();
	MoveKinematic();
	UpdateFishing();
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_self->GetID() == "fishingZone" && _other->GetOwner()->GetTag() == 5)
		m_fishingZoneTouchingWater = true;

	if (_other->GetOwner()->GetTag() == 2 && _self->GetID() == "actionBox") 
	{
		if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		{
			EnviroProps* props = (EnviroProps*)_other->GetOwner();
			TryHitEnviroProps(props);
		}
	}
}

void Player::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_self->GetID() == "fishingZone" && _other->GetOwner()->GetTag() == 5)
		m_fishingZoneTouchingWater = true;

	if (_other->GetOwner()->GetTag() == 2 && _self->GetID() == "actionBox") 
	{
		if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		{
			EnviroProps* props = (EnviroProps*)_other->GetOwner();
			TryHitEnviroProps(props);
		}
	}
}

void Player::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{

}

PlayerJob Player::GetJobByName(JobType p_type)
{
	for (PlayerJob job : gm.GetPlayerData().m_jobs)
	{
		if (job.m_name == p_type)
			return job ;
	}

}

void Player::UpdateJobs()
{
	/*
	for (int i = 0; i < gm.GetPlayerData().m_jobs.size(); i++) {
		gm.GetPlayerData().m_jobs[i].levelUp();
		switch (gm.GetPlayerData().m_jobs[i].m_name)
		{
		case JobType::Gatherer:
			switch (gm.GetPlayerData().m_jobs[i].m_lvl) {
			case 2:
				UnlockRecipe(Recipes::BandageL1);
				break;
			case 3:
				UnlockRecipe(Recipes::EnergizingL1);
				break;
			case 4:
				gm.GetPlayerData().m_stats.m_harvestEffiency *= 1.067f;
				break;
			case 5:
				UnlockRecipe(Recipes::BandageL2);
				break;
			case 6:
				UnlockRecipe(Recipes::EnergizingL2);
				break;
			}
			break;

		case JobType::Farmer:
			switch (gm.GetPlayerData().m_jobs[i].m_lvl) {
			case 2:
				UnlockRecipe(Recipes::FertilizerL1);
				break;
			case 3:
				UnlockRecipe(Recipes::AutoWateringL1);

				break;
			case 4:
				gm.GetPlayerData().m_stats.m_farmerSpeed *= 1.067f;
				break;
			case 5:
				UnlockRecipe(Recipes::FertilizerL2);

				break;
			case 6:
				UnlockRecipe(Recipes::AutoWateringL2);
				break;
			}
			break;

		case JobType::Miner:
			switch (gm.GetPlayerData().m_jobs[i].m_lvl) {
			case 2:
				UnlockRecipe(Recipes::StoneTools);

				break;
			case 3:
				UnlockRecipe(Recipes::IronTools);

				break;
			case 4:
				gm.GetPlayerData().m_stats.m_miningSpeed *= 1.067f;
				break;
			case 5:
				UnlockRecipe(Recipes::GoldTools);

				break;
			case 6:
				UnlockRecipe(Recipes::DiamondTools);

				break;
			}
			break;

		case JobType::Dealer:
			switch (gm.GetPlayerData().m_jobs[i].m_lvl) {
			case 2:
				gm.GetPlayerData().m_stats.m_drugsSellQuantity += 15;
				break;
			case 3:
				UnlockRecipe(Recipes::WeedSeed);
				break;
			case 4:
				gm.GetPlayerData().m_stats.m_drugSellEfficency *= 1.067f;
				break;
			case 5:
				gm.GetPlayerData().m_stats.m_drugsSellQuantity += 40;
				break;
			case 6:
				UnlockRecipe(Recipes::CannabisSeed);
				break;
			}
			break;

		case JobType::Killer:
			switch (gm.GetPlayerData().m_jobs[i].m_lvl) {
			case 2:
				UnlockRecipe(Recipes::Knife);
				break;
			case 3:
				UnlockRecipe(Recipes::Sword);
				break;
			case 4:
				gm.GetPlayerData().m_stats.m_playerSpeed *= 1.067f;
				break;
			case 5:
				UnlockRecipe(Recipes::Glock);
				break;
			case 6:
				UnlockRecipe(Recipes::Rifle);
				break;
			}
			break;
		}
	}
	*/
}

void Player::SetupStateMachine()
{ 
	m_stateMachine.AddState(new PlayerIdleState);//0
	m_stateMachine.AddState(new PlayerWalkingState);//1
	m_stateMachine.AddState(new PlayerWateringState);//2
	m_stateMachine.AddState(new PlayerDiggingState);//3
	m_stateMachine.AddState(new PlayerMiningState);//4

	//TRANSITIONS 

	//IDLE
	{
		// to digging
		auto transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::DIGGING);
		transition->AddCondition(new HasSpadeInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		// to mining
		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::MINING);
		transition->AddCondition(new HasPickaxeInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		// to watering
		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::WATERING);
		transition->AddCondition(new HasWateringCanInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		// to walk states
		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::WALKING);
		Condition<Player>* playerStaticCondition = new IsPlayerStatic;
		playerStaticCondition->ExpectedValue = false;
		transition->AddCondition(playerStaticCondition);
	}

	//DIGGING 
	{
		auto transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::IDLE);
		Condition<Player>* hasSpadeInHandsCondition = new HasSpadeInHands;
		hasSpadeInHandsCondition->ExpectedValue = false;
		transition->AddCondition(hasSpadeInHandsCondition);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::IDLE);
		Condition<Player>* digToIdleCondition = new IsPlayerUsingToolInHand;
		digToIdleCondition->ExpectedValue = false;
		transition->AddCondition(digToIdleCondition);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::MINING);
		transition->AddCondition(new HasPickaxeInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WATERING);
		transition->AddCondition(new HasWateringCanInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WALKING);
		Condition<Player>* isStaticCondition = new IsPlayerStatic;
		isStaticCondition->ExpectedValue = false;
		transition->AddCondition(isStaticCondition);
	}

	//WATERING
	{
		auto transition = m_stateMachine.AddTransition((int)PlayerState::WATERING, (int)PlayerState::IDLE);
		Condition<Player>* hasWateringCanCondition = new HasWateringCanInHands;
		hasWateringCanCondition->ExpectedValue = false;
		transition->AddCondition(hasWateringCanCondition);

		transition = m_stateMachine.AddTransition((int)PlayerState::WATERING, (int)PlayerState::IDLE);
		Condition<Player>* playerUsingToolCondition = new IsPlayerUsingToolInHand;
		playerUsingToolCondition->ExpectedValue = false;
		transition->AddCondition(playerUsingToolCondition);

		transition = m_stateMachine.AddTransition((int)PlayerState::WATERING, (int)PlayerState::MINING);
		transition->AddCondition(new HasWateringCanInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::WATERING, (int)PlayerState::DIGGING);
		transition->AddCondition(new HasSpadeInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::WATERING, (int)PlayerState::WALKING);
		Condition<Player>* playerStaticCondition = new IsPlayerStatic;
		playerStaticCondition->ExpectedValue = false;
		transition->AddCondition(playerStaticCondition);
	}

	//MINING 
	{
		auto transition = m_stateMachine.AddTransition((int)PlayerState::MINING, (int)PlayerState::IDLE);
		Condition<Player>* playerUsingToolCondition = new IsPlayerUsingToolInHand;
		playerUsingToolCondition->ExpectedValue = false;
		transition->AddCondition(playerUsingToolCondition);

		transition = m_stateMachine.AddTransition((int)PlayerState::MINING, (int)PlayerState::IDLE);
		Condition<Player>* hasPickaxeCondition = new HasPickaxeInHands;
		hasPickaxeCondition->ExpectedValue = false;
		transition->AddCondition(hasPickaxeCondition);

		transition = m_stateMachine.AddTransition((int)PlayerState::MINING, (int)PlayerState::DIGGING);
		transition->AddCondition(new HasSpadeInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::MINING, (int)PlayerState::WATERING);
		transition->AddCondition(new HasWateringCanInHands);
		transition->AddCondition(new IsPlayerUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::MINING, (int)PlayerState::WALKING);
		Condition<Player>* isStaticCondition = new IsPlayerStatic;
		isStaticCondition->ExpectedValue = false;
		transition->AddCondition(isStaticCondition);

	}

	SetState(PlayerState::WALKING);
}

void Player::LockInputs()
{
	m_inputBlocker = true;
}

void Player::UnlockInputs()
{
	m_inputBlocker = false;
}

void Player::Heal(int p_amount)
{
	gm.GetPlayerData().m_stats.m_hp += p_amount;
	if (gm.GetPlayerData().m_stats.m_hp > gm.GetPlayerData().m_stats.m_maxHp)
		gm.GetPlayerData().m_stats.m_hp = gm.GetPlayerData().m_stats.m_maxHp;
}

void Player::TakeDamage(int p_amount)
{
	gm.GetPlayerData().m_stats.m_hp -= p_amount;
	if (gm.GetPlayerData().m_stats.m_hp < 0)
		gm.GetPlayerData().m_stats.m_hp = 0;
}

void Player::DecreaseStamina(int p_amount)
{
	gm.GetPlayerData().m_stats.m_stamina -= p_amount;
	if (gm.GetPlayerData().m_stats.m_stamina < 0)
		gm.GetPlayerData().m_stats.m_stamina = 0;
}

void Player::AddStamina(int p_amount)
{
	gm.GetPlayerData().m_stats.m_stamina += p_amount;
	if (gm.GetPlayerData().m_stats.m_stamina > gm.GetPlayerData().m_stats.m_maxStamina)
		gm.GetPlayerData().m_stats.m_stamina = gm.GetPlayerData().m_stats.m_maxStamina;
}

void Player::DecreaseToxicity(int p_amount)
{
	gm.GetPlayerData().m_stats.m_toxicity -= p_amount;
	if (gm.GetPlayerData().m_stats.m_toxicity < 0)
		gm.GetPlayerData().m_stats.m_toxicity = 0;
}

void Player::AddToxicity(int p_amount)
{
	gm.GetPlayerData().m_stats.m_toxicity += p_amount;
	if (gm.GetPlayerData().m_stats.m_toxicity > gm.GetPlayerData().m_stats.m_maxToxicity)
		gm.GetPlayerData().m_stats.m_toxicity = gm.GetPlayerData().m_stats.m_maxToxicity;
}

void Player::DecreaseMoney(int p_amount)
{
	gm.GetPlayerData().m_money -= p_amount;
	if (gm.GetPlayerData().m_money < 0)
		gm.GetPlayerData().m_money = 0;
}

void Player::DecreaseCash(int p_amount)
{
	gm.GetPlayerData().m_cash -= p_amount;
	if (gm.GetPlayerData().m_cash < 0)
		gm.GetPlayerData().m_cash = 0;
}

void Player::SetState(PlayerState p_state)
{
	m_stateMachine.TryTransitionTo(this, (int)p_state);
}