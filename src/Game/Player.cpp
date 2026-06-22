#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

#include "FarmManager.h"   
#include "GameData.h" 

#include "PlayerState.h"
#include "PlayerConditions.h"

void Player::OnInitialize()
{
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
	m_Jobs.push_back(killer);
	PlayerJob* dealer = new PlayerJob();
	dealer->m_name = JobType::Dealer;
	m_Jobs.push_back(dealer);*/


	SetupStateMachine();

}

void Player::OnEvent()
{
	/*if (InputManager::Get()->IsKeyboardDown(K_n))
		hasWateringCan = !hasWateringCan;*/

		/*if (InputManager::Get()->IsKeyboardDown(K_i))
			hasPickaxe = !hasPickaxe;*/

	if (InputManager::Get()->IsKeyboardDown(K_b))
	{
		hasSpade = !hasSpade;
		std::cout << "hasspade : " << std::to_string(hasSpade) << std::endl;
	}
	if (InputManager::Get()->IsKeyboardDown(K_u))
	{
		SetIsUsingToolInHand(!m_isUsingToolInHand);
		std::cout << "is using : " << std::to_string(m_isUsingToolInHand) << std::endl;
	}

	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(m_stats.m_playerSpeed * 2);
	}
	else SetSpeed(m_stats.m_playerSpeed);

	Vector2f direction = GetDirection();
	Collider* boxCollider = GetCollider("actionBox");
	//Input Clavier
	// lock les mouvements pendant les dialogues
	if (m_isInDialog == false)
	{
		//Input Clavier
		if (InputManager::Get()->IsKeyboardHeld(K_z)) {
			SetDirection({ GetDirection().GetX(), -1.0f });
			GetCollider("actionBox")->SetOffset({ 0, -8 });
		}
		if (InputManager::Get()->IsKeyboardHeld(K_s)) {
			SetDirection({ GetDirection().GetX(), 1.0f });
			GetCollider("actionBox")->SetOffset({ 0, 24 });
		}
		if (InputManager::Get()->IsKeyboardHeld(K_q)) {
			SetDirection({ -1.0f, GetDirection().GetY() });
			GetCollider("actionBox")->SetOffset({ -16, 8 });
		}
		if (InputManager::Get()->IsKeyboardHeld(K_d)) {
			SetDirection({ 1.0f, GetDirection().GetY() });
			GetCollider("actionBox")->SetOffset({ 16, 8 });
		}
		//input souris
		if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
			//utilise l'objet selectionner dans l'inventaire (Vivien a l'aide)
		}

	}

	//input souris
	if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
		//utilise l'objet selectionner dans l'inventaire (Vivien a l'aide)
	}


	// TEMP - AGRI

	// S�lection plante 
	if (InputManager::Get()->IsKeyboardDown(K_1))
		m_selectedCrop = CropCatalog::Wheat();
	if (InputManager::Get()->IsKeyboardDown(K_2))
		m_selectedCrop = CropCatalog::Carrot();

	// Action farm 
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		HandleFarmAction();

	/*if (InputManager::Get()->IsKeyboardDown(K_u))*/

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
		if (InputManager::Get()->IsKeyboardHeld(K_t))
			target->Water(currentDay);
		else if (InputManager::Get()->IsKeyboardHeld(K_y))
			target->Plant(m_selectedCrop, currentDay);
		else
			target->Till(currentDay);
		break;

	case FarmTileState::WATERED:
		// Y maintenu = planter
		if (InputManager::Get()->IsKeyboardHeld(K_y))
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

void Player::OnUpdate()
{
	UpdateJobs();
	m_stateMachine.Update(this, GetDeltaTime());
	MoveKinematic();
}

void Player::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{

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
	m_stateMachine.AddState(new PlayerWalkDownState);//1
	m_stateMachine.AddState(new PlayerWalkUpState);//2
	m_stateMachine.AddState(new PlayerWalkRightState);//3
	m_stateMachine.AddState(new PlayerWalkLeftState);//4
	m_stateMachine.AddState(new PlayerWateringState);//5
	m_stateMachine.AddState(new PlayerDiggingState);//6
	m_stateMachine.AddState(new PlayerMiningState);//7

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
		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::WALKUP);
		transition->AddCondition(new IsPlayerGoingUp);

		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::WALKDOWN);
		transition->AddCondition(new IsPlayerGoingDown);

		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::WALKLEFT);
		transition->AddCondition(new IsPlayerGoingLeft);

		transition = m_stateMachine.AddTransition((int)PlayerState::IDLE, (int)PlayerState::WALKRIGHT);
		transition->AddCondition(new IsPlayerGoingRight);
	}

	//DIGGING
	{
		auto transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::IDLE);
		transition->AddCondition(new DontHaveSpadeInHands);
		transition->AddCondition(new IsPlayerNotUsingToolInHand);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::MINING);
		transition->AddCondition(new IsPlayerNotUsingToolInHand);
		transition->AddCondition(new HasPickaxeInHands);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WATERING);
		transition->AddCondition(new IsPlayerNotUsingToolInHand);
		transition->AddCondition(new HasWateringCanInHands);


		// to walk states
		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WALKDOWN);
		transition->AddCondition(new IsPlayerGoingDown);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WALKUP);
		transition->AddCondition(new IsPlayerGoingUp);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WALKLEFT);
		transition->AddCondition(new IsPlayerGoingLeft);

		transition = m_stateMachine.AddTransition((int)PlayerState::DIGGING, (int)PlayerState::WALKRIGHT);
		transition->AddCondition(new IsPlayerGoingRight);


	}

	//WALK UP
	{
		auto transition = m_stateMachine.AddTransition((int)PlayerState::WALKUP, (int)PlayerState::IDLE);
		transition->AddCondition(new IsPlayerStatic);

		transition = m_stateMachine.AddTransition((int)PlayerState::WALKUP, (int)PlayerState::WALKDOWN);
		transition->AddCondition(new IsPlayerGoingDown);
	}

	//WALK DOWN
	{
		m_stateMachine.AddTransition((int)PlayerState::WALKDOWN, (int)PlayerState::IDLE)->AddCondition(new IsPlayerStatic);
		m_stateMachine.AddTransition((int)PlayerState::WALKDOWN, (int)PlayerState::WALKUP)->AddCondition(new IsPlayerGoingUp);


	}

	//WALK LEFT
	{
		m_stateMachine.AddTransition((int)PlayerState::WALKLEFT, (int)PlayerState::IDLE)->AddCondition(new IsPlayerStatic);
		m_stateMachine.AddTransition((int)PlayerState::WALKLEFT, (int)PlayerState::WALKUP)->AddCondition(new IsPlayerGoingUp);
		m_stateMachine.AddTransition((int)PlayerState::WALKLEFT, (int)PlayerState::WALKDOWN)->AddCondition(new IsPlayerGoingDown);
		m_stateMachine.AddTransition((int)PlayerState::WALKLEFT, (int)PlayerState::WALKRIGHT)->AddCondition(new IsPlayerGoingRight);

	}

	//WALK RIGHT
	{
		auto transition = m_stateMachine.AddTransition((int)PlayerState::WALKRIGHT, (int)PlayerState::WALKUP);
		transition->AddCondition(new IsPlayerGoingUp);

		transition = m_stateMachine.AddTransition((int)PlayerState::WALKRIGHT, (int)PlayerState::WALKDOWN);
		transition->AddCondition(new IsPlayerGoingDown);

		transition = m_stateMachine.AddTransition((int)PlayerState::WALKRIGHT, (int)PlayerState::WALKLEFT);
		transition->AddCondition(new IsPlayerGoingLeft);

		transition = m_stateMachine.AddTransition((int)PlayerState::WALKRIGHT, (int)PlayerState::IDLE);
		transition->AddCondition(new IsPlayerStatic);

	}

	SetState(PlayerState::IDLE);
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

void Player::SetState(PlayerState p_state)
{
	m_stateMachine.TryTransitionTo(this, (int)p_state);
}
