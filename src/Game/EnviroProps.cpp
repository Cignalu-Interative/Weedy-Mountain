#include "pch.h"
#include "EnviroProps.h"
#include "Collision.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"
#include "SpriteContainer.h"
#include "ItemOnGround.h"


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
	for (int i = 0; i < m_Loots.size(); i++) {

		int numLoots = Aleatory::GetInt(m_Loots[i].m_MinDrop, m_Loots[i].m_MaxDrop);

		for (int j = 0; j < numLoots; j++) {
			CreateItem(i);
		}
	}
	
}

void EnviroProps::CreateItem(int lootPos)
{
	Scene* sc = GetScene();
	float posX = GetPosition().GetX();
	float posY = GetPosition().GetY();

	ItemOnGround* Loot;
	Loot = sc->CreateEntity<ItemOnGround>({ Aleatory::GetFloat(posX,posX + 50),Aleatory::GetFloat(posY, posY + 50) }, { 16,16 }, "", Shape::Rectangle);
	Loot->SetItem(&m_Loots[lootPos].m_item);
}

void EnviroProps::SetType(EnviroSpawnName p_name)
{
	EnviroLoots Loot1{ (ItemStack(ITEM_WOOD,1)), 3,4 };
	EnviroLoots Loot2{ (ItemStack(ITEM_FIBER,1)), 1,2 };
	switch (p_name) {
	case EnviroSpawnName::Tree:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 4;
		m_Stats.m_MinToolsRarity = Rarity::Stone;
		m_Stats.m_ToolType = { ToolType::Axe };
		Loot1 = { (ItemStack(ITEM_WOOD,1)), 3,4 };
		Loot2 = { (ItemStack(ITEM_FIBER,1)), 1,2 };
		m_Loots.push_back(Loot1);
		m_Loots.push_back(Loot2);
		break;
	case EnviroSpawnName::Shrub:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 2;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Axe };
		Loot1 = { (ItemStack(ITEM_WOOD,1)), 1,2 };
		Loot2 = { (ItemStack(ITEM_FIBER,1)), 0,2 };
		m_Loots.push_back(Loot1);
		m_Loots.push_back(Loot2);
		break;
	case EnviroSpawnName::Grass:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 1;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Hoe, ToolType::Spade};
		Loot1 = { (ItemStack(ITEM_FIBER,1)), 1,3 };
		m_Loots.push_back(Loot1);
		break;
	case EnviroSpawnName::Rock:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 2;
		m_Stats.m_MinToolsRarity = Rarity::Wooden;
		m_Stats.m_ToolType = { ToolType::Pickaxe };
		Loot1 = { (ItemStack(ITEM_ROCK,1)), 1,3 };
		m_Loots.push_back(Loot1);
		break;
	case EnviroSpawnName::Stone:
		m_Name = p_name;
		m_Stats.m_HitToDestroy = 4;
		m_Stats.m_MinToolsRarity = Rarity::Iron;
		m_Stats.m_ToolType = { ToolType::Pickaxe };
		Loot1 = { (ItemStack(ITEM_ROCK,1)), 2,5 };
		m_Loots.push_back(Loot1);
		break;
	}
}
