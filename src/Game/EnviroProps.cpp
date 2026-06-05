#include "pch.h"
#include "EnviroProps.h"
#include "Collision.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"

void EnviroProps::OnInitialize()
{
	
}

void EnviroProps::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_RIGHT) && m_isInPlayerZone){

	}
}

void EnviroProps::OnUpdate()
{

}

void EnviroProps::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if(_other->GetOwner()->GetTag() == 1 && _other->GetID() == "interact") {
		m_isInPlayerZone = true;
	}
}

void EnviroProps::OnDestroy()
{
}

void EnviroProps::SetType(EnviroSpawnName p_name)
{

	switch (p_name) {
	case EnviroSpawnName::Tree:
		m_Name = EnviroName::Tree;
		m_Stats.m_HitToDestroy = 4;
		m_Stats.m_MinToolsRarity = Rarity::Stone;
		m_Stats.m_ToolType = { ToolType::Axe };
		break;
	case EnviroSpawnName::Shrub:
		m_Name = EnviroName::Shrub;
		m_Stats.m_HitToDestroy = 2;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Axe };
		break;
	case EnviroSpawnName::Grass:
		m_Name = EnviroName::Grass;
		m_Stats.m_HitToDestroy = 1;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Hoe, ToolType::Spade};
		break;
	case EnviroSpawnName::Rock:
		m_Name = EnviroName::Rock;
		m_Stats.m_HitToDestroy = 2;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Pickaxe };
		break;
	case EnviroSpawnName::Stone:
		m_Name = EnviroName::Stone;
		m_Stats.m_HitToDestroy = 4;
		m_Stats.m_MinToolsRarity = Rarity::Iron;
		m_Stats.m_ToolType = { ToolType::Pickaxe };
		break;
	}
}
