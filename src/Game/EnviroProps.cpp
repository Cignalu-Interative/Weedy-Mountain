#include "pch.h"
#include "EnviroProps.h"
#include "Collision.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"
#include "SpriteContainer.h"

void EnviroProps::OnInitialize()
{
	
}

void EnviroProps::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT) && m_isInPlayerZone){
		// ajouter la verification de l'outils et de sa rareté
		m_Stats.m_HitToDestroy--;
	}

	if (m_Stats.m_HitToDestroy <= 0) {
		Destroy();
	}

	Vector2f playerPos = SceneManager::Get()->GetScene("game")->GetEntityByTag(1)->GetPosition();

	if (playerPos.GetY() < GetPosition().GetY()+16) {
		SetDrawLayer(3);

	}
	else SetDrawLayer(1);

	if (playerPos.GetX() > GetPosition().GetX() - 30 && playerPos.GetX() < GetPosition().GetX() + 30 && GetDrawLayer() == 3 && playerPos.GetY() > GetPosition().GetY() - GetSprite()->GetDimension().GetY()/2) {
		GetSprite()->SetOpacity(100);
	}
	else GetSprite()->SetOpacity(255);


}

void EnviroProps::OnUpdate()
{

}

void EnviroProps::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
	if(_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = true;
	}

}

void EnviroProps::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
	if (_other->GetOwner()->GetTag() == 1 && _other->GetID() == "actionBox") {
		m_isInPlayerZone = false;
	}
}

void EnviroProps::OnDestroy()
{
}

void EnviroProps::SetType(EnviroSpawnName p_name)
{

	switch (p_name) {
	case EnviroSpawnName::Tree:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 4;
		m_Stats.m_MinToolsRarity = Rarity::Stone;
		m_Stats.m_ToolType = { ToolType::Axe };
		break;
	case EnviroSpawnName::Shrub:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 2;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Axe };
		break;
	case EnviroSpawnName::Grass:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 1;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Hoe, ToolType::Spade};
		break;
	case EnviroSpawnName::Rock:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 2;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Pickaxe };
		break;
	case EnviroSpawnName::Stone:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 4;
		m_Stats.m_MinToolsRarity = Rarity::Iron;
		m_Stats.m_ToolType = { ToolType::Pickaxe };
		break;
	}
}
