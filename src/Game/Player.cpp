#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

void Player::OnInitialize()
{
	PlayerJob* gatherer = new PlayerJob();
	gatherer->m_name = JobType::Gatherer;
	m_Jobs.push_back(gatherer);
	PlayerJob* farmer = new PlayerJob();
	farmer->m_name = JobType::Farmer;
	m_Jobs.push_back(farmer);
	PlayerJob* miner = new PlayerJob();
	miner->m_name = JobType::Miner;
	m_Jobs.push_back(miner);
	/*PlayerJob* killer = new PlayerJob();
	killer->m_name = JobType::Killer;
	m_Jobs.push_back(killer);
	PlayerJob* dealer = new PlayerJob();
	dealer->m_name = JobType::Dealer;
	m_Jobs.push_back(dealer);*/
}

void Player::OnEvent()
{
	if (InputManager::Get()->IsKeyboardHeld(K_LSHIFT)) {
		SetSpeed(m_Stats.m_PlayerSpeed * 2);
	}
	else SetSpeed(m_Stats.m_PlayerSpeed);

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

	if (InputManager::Get()->IsKeyboardDown(K_x))
	{
		for (int i = 0; i < GetJobs().size(); i++)
		{
			GetJobs()[i]->m_exp = GetJobs()[i]->m_expNeeded;
		}
	}
}

void Player::OnUpdate()
{
	UpdateJobs();
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{

}

void Player::OnDestroy()
{

}

void Player::UpdateJobs()
{
	for (int i = 0; i < m_Jobs.size(); i++) {
		m_Jobs[i]->levelUp();
		switch (m_Jobs[i]->m_name)
		{
		case JobType::Gatherer:
			switch (m_Jobs[i]->m_lvl) {
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

		case JobType::Farmer:
			switch (m_Jobs[i]->m_lvl) {
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
			switch (m_Jobs[i]->m_lvl) {
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
			switch (m_Jobs[i]->m_lvl) {
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
			switch (m_Jobs[i]->m_lvl) {
			case 2:
				UnlockRecipe(Recipes::Knife);
				break;
			case 3:
				UnlockRecipe(Recipes::Sword);
				break;
			case 4:
				GetStats().m_PlayerSpeed *= 1.067f;
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
