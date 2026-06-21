#include "pch.h"
#include "Player.h"
#include "Scene.h"
#include "DemoScene.h"
#include "2DPendentLib/InputManager.h"

#include "User.h"

#include "FarmManager.h"   
#include "GameData.h" 
#include "Inventory.h"

void Player::OnInitialize()
{

	m_inventory = GetScene()->CreateEntity<Inventory>(GetPosition(), {636, 210}, "InventoryWeedyMountain", Shape::Rectangle, true);
	m_inventory->SetVisible(false);
	//TO DO suppress
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
	m_Jobs.push_back(killer);
	PlayerJob* dealer = new PlayerJob();
	dealer->m_name = JobType::Dealer;
	m_Jobs.push_back(dealer);*/
}

void Player::OnEvent()
{
	float currentSpeed = InputManager::Get()->IsKeyboardHeld(K_LSHIFT) ? m_stats.m_playerSpeed * 2.f : m_stats.m_playerSpeed;
	SetSpeed(currentSpeed);

	if (InputManager::Get()->IsKeyboardDown(K_TAB))
	{
		m_inventory->SetVisible(!m_inventory->GetIsVisible());
	}

	Vector2f dir = {0.f, 0.f};
	Collider* boxCollider = GetCollider("actionBox");
	//Input Clavier
	// lock les mouvements pendant les dialogues
	if (m_isInDialog == false)
	{
		//Input Clavier
		if (InputManager::Get()->IsKeyboardHeld(K_z))
		{
			dir.SetY(dir.GetY() - 1.f);
			boxCollider->SetOffset({ 0, -8 });
		}
		if (InputManager::Get()->IsKeyboardHeld(K_s)) 
		{
			dir.SetY(dir.GetY() + 1.f);
			boxCollider->SetOffset({ 0, 24 });
		}
		if (InputManager::Get()->IsKeyboardHeld(K_q)) 
		{
			dir.SetX(dir.GetX() - 1.f);
			boxCollider->SetOffset({ -16, 8 });
		}
		if (InputManager::Get()->IsKeyboardHeld(K_d))
		{
			dir.SetX(dir.GetX() + 1.f);
			boxCollider->SetOffset({ 16, 8 });
		}
		//input souris
		if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT)) {
			//utilise l'objet selectionner dans l'inventaire (Vivien a l'aide)
		}

	}

	float len = std::sqrtf(dir.GetX() * dir.GetX() + dir.GetY() * dir.GetY());
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

	// Action farm 
	/*if (InputManager::Get()->IsMouseDown(BUTTON_LEFT))
		HandleFarmAction();*/
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

	MoveKinematic();
}

void Player::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side)
{
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
