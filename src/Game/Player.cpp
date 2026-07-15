#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

#include "FarmManager.h"   
#include "GameData.h" 
#include "Inventory.h"

#include "PlayerState.h"
#include "PlayerConditions.h"

#include "Aleatory.h"

void Player::OnInitialize()
{

	m_inventory = GetScene()->CreateEntity<Inventory>({}, {636, 210}, "InventoryWeedyMountain", Shape::Rectangle, true);
	m_inventory->SetVisible(false);
	m_inventory->AddItem("ITEM_WOOD", 10, false);
	
	PlayerJob* gatherer = new PlayerJob();
	gatherer->m_name = JobType::Gatherer;
	m_jobs.push_back(gatherer);

	PlayerJob* farmer = new PlayerJob();
	farmer->m_name = JobType::Farmer;
	m_jobs.push_back(farmer);

	PlayerJob* miner = new PlayerJob();
	miner->m_name = JobType::Miner;
	m_jobs.push_back(miner);

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
	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(m_stats.m_playerSpeed * 2);
	}
	else SetSpeed(m_stats.m_playerSpeed);

	float currentSpeed = InputManager::Get()->IsKeyboardHeld(K_LSHIFT) ? m_stats.m_playerSpeed * 2.f : m_stats.m_playerSpeed;
	SetSpeed(currentSpeed);

	if (InputManager::Get()->IsKeyboardDown(K_TAB))
	{
		m_inventory->SetVisible(!m_inventory->GetIsVisible());
	}

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

		//input souris
		if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
			//utilise l'objet selectionner dans l'inventaire (Vivien a l'aide)
		}

		if (InputManager::Get()->IsKeyboardDown(K_1))
			m_selectedCrop = CropCatalog::Wheat();
		if (InputManager::Get()->IsKeyboardDown(K_2))
			m_selectedCrop = CropCatalog::Carrot();

		// Action farm 
		// TEMP - AGRI

		// S�lection plante 
		if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
			HandleFarmAction();

		// TEMP - PECHE
		if (InputManager::Get()->IsKeyboardDown(K_F))
			StartFishing();
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

	// TEMP - AGRI

	// S�lection plante 
	if (InputManager::Get()->IsKeyboardDown(K_1))
		m_selectedCrop = CropCatalog::Wheat();
	if (InputManager::Get()->IsKeyboardDown(K_2))
		m_selectedCrop = CropCatalog::Carrot();
}

// TEMP - AGRI
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
		// B�cher
		target->Till(currentDay);
		break;

	case FarmTileState::TILLED:
		// T maintenu = arroser, Y maintenu = planter, sinon = d�-b�cher
		if (InputManager::Get()->IsKeyboardHeld(K_T))
			target->Water(currentDay);
		else if (InputManager::Get()->IsKeyboardHeld(K_Y))
			target->Plant(m_selectedCrop, currentDay);
		else
			target->Till(currentDay);
		break;

	case FarmTileState::WATERED:
		// Y maintenu = planter
		if (InputManager::Get()->IsKeyboardHeld(K_Y))
			target->Plant(m_selectedCrop, currentDay);
		break;

	case FarmTileState::PLANTED:
	case FarmTileState::GROWING:
		// Arroser
		target->Water(currentDay);
		break;

	case FarmTileState::READY:
		// R�colter
		target->Harvest(currentDay);
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
			m_fishingTimerTarget = Aleatory::GetFloat(2.f, 5.f);
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
			std::cout << "[Fishing] poisson attrape !" << std::endl;
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

	UpdateJobs();
	MoveKinematic();
	UpdateFishing();
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_self->GetID() == "fishingZone" && _other->GetOwner()->GetTag() == 5)
		m_fishingZoneTouchingWater = true;
}

void Player::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_self->GetID() == "fishingZone" && _other->GetOwner()->GetTag() == 5)
		m_fishingZoneTouchingWater = true;
}

void Player::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{

}

void Player::UpdateJobs()
{
	for (int i = 0; i < m_jobs.size(); i++) {
		m_jobs[i]->levelUp();
		switch (m_jobs[i]->m_name)
		{
		case JobType::Gatherer:
			switch (m_jobs[i]->m_lvl) {
			case 2:
				UnlockRecipe(Recipes::BandageL1);
				break;
			case 3:
				UnlockRecipe(Recipes::EnergizingL1);
				break;
			case 4:
				GetStats().m_harvestEffiency *= 1.067f;
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
			switch (m_jobs[i]->m_lvl) {
			case 2:
				UnlockRecipe(Recipes::FertilizerL1);
				break;
			case 3:
				UnlockRecipe(Recipes::AutoWateringL1);

				break;
			case 4:
				GetStats().m_farmerSpeed *= 1.067f;
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
			switch (m_jobs[i]->m_lvl) {
			case 2:
				UnlockRecipe(Recipes::StoneTools);

				break;
			case 3:
				UnlockRecipe(Recipes::IronTools);

				break;
			case 4:
				GetStats().m_miningSpeed *= 1.067f;
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
			switch (m_jobs[i]->m_lvl) {
			case 2:
				GetStats().m_drugsSellQuantity += 15;
				break;
			case 3:
				UnlockRecipe(Recipes::WeedSeed);
				break;
			case 4:
				GetStats().m_drugSellEfficency *= 1.067f;
				break;
			case 5:
				GetStats().m_drugsSellQuantity += 40;
				break;
			case 6:
				UnlockRecipe(Recipes::CannabisSeed);
				break;
			}
			break;

		case JobType::Killer:
			switch (m_jobs[i]->m_lvl) {
			case 2:
				UnlockRecipe(Recipes::Knife);
				break;
			case 3:
				UnlockRecipe(Recipes::Sword);
				break;
			case 4:
				GetStats().m_playerSpeed *= 1.067f;
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
}

void Player::UnlockRecipe(Recipes _toUnlock)
{
	if (!IsRecipeUnlocked(_toUnlock))
	{
		m_unlockedRecipes.push_back(_toUnlock);
		std::cout << "item unlocked" << std::endl;
	}

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

bool Player::IsRecipeUnlocked(Recipes _toUnlock)
{
	for (int i = 0; i < m_unlockedRecipes.size(); i++)
	{
		if (m_unlockedRecipes[i] == _toUnlock)
		{
			return true;
		}

	}
	return false;
}

void Player::LockInputs()
{
	m_inputBlocker = true;
}

void Player::UnlockInputs()
{
	m_inputBlocker = false;
}

void Player::SetState(PlayerState p_state)
{
	m_stateMachine.TryTransitionTo(this, (int)p_state);
}
