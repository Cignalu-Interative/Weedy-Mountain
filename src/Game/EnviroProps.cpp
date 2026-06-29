#include "pch.h"
#include "EnviroProps.h"
#include "Collision.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"
#include "SpriteContainer.h"
#include "ItemOnGround.h"
#include "ItemDataBase.h"


void EnviroProps::OnInitialize()
{
	
}

void EnviroProps::OnEvent()
{
	if (InputManager::Get()->IsMouseDown(BUTTON_LEFT) && m_isInPlayerZone){
		// ajouter la verification de l'outils et de sa raret�
		m_stats.m_hitToDestroy--;
	}

	if (m_stats.m_hitToDestroy <= 0) {
		Destroy();
	}

	Vector2f playerPos = SceneManager::Get()->GetScene("game")->GetEntityByTag(1)->GetPosition();

	if (playerPos.GetY() < GetPosition().GetY()+16) {
		SetDrawLayer(3);

	}
	else SetDrawLayer(1);

	if (playerPos.GetX() > GetPosition().GetX() - 30 && playerPos.GetX() < GetPosition().GetX() + 30 && GetDrawLayer() == 3 && playerPos.GetY() > GetPosition().GetY() - GetSprite()->GetDimension().GetY()/2) {
		if(Sprite* sprite = GetSprite("default"))sprite->SetOpacity(100);
	}
	else {
		if (Sprite* sprite = GetSprite("default"))sprite->SetOpacity(255);
	}

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
	for (int i = 0; i < m_loots.size(); i++) {

		int numLoots = Aleatory::GetInt(m_loots[i]->m_minDrop, m_loots[i]->m_maxDrop);

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
	std::string path = ItemDataBase::Get()->GetItem(m_loots[lootPos]->m_item.GetID())->GetPath();
	Loot = sc->CreateEntity<ItemOnGround>({ Aleatory::GetFloat(posX,posX + 50),Aleatory::GetFloat(posY, posY + 50) }, { 16,16 }, path, Shape::Circle);
	Loot->SetItem(m_loots[lootPos]->m_item);
	Loot->SetTag(3);
	Loot->GetCollider()->Resize({ 20,20 });
	Loot->ShowCollider(true);
	Loot->InitialisePhysicsBody(ShapeBodyPhysics::Kinematic);
	Loot->GetCollider()->SetIsTrigger(true);
}

void EnviroProps::SetType(EnviroSpawnName p_name)
{
	switch (p_name) {
	case EnviroSpawnName::Tree:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 4;
		m_stats.m_minToolsRarity = Rarity::Stone;
		m_stats.m_toolType = { ToolType::Axe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack(ITEM_WOOD,1)), 3,4 };
		EnviroLoots* loot2 = new EnviroLoots{ (ItemStack(ITEM_FIBER,1)), 1,2 };
		m_loots.push_back(loot1);
		//m_Loots.push_back(loot2);
		break;
	}
	case EnviroSpawnName::Shrub:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = Rarity::Wooden;
		m_stats.m_toolType = { ToolType::Axe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack(ITEM_WOOD,1)), 1,2 };
		EnviroLoots* Loot2 = new EnviroLoots{ (ItemStack(ITEM_FIBER,1)), 0,2 };
		m_loots.push_back(loot1);
		m_loots.push_back(Loot2);
		break;
	}
	case EnviroSpawnName::Grass:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 1;
		m_stats.m_minToolsRarity = Rarity::Wooden;
		m_stats.m_toolType = { ToolType::Hoe, ToolType::Spade };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack(ITEM_FIBER,1)), 1,3 };
		m_loots.push_back(loot1);
		break;
	}
	case EnviroSpawnName::Rock:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = Rarity::Wooden;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack(ITEM_ROCK,1)), 1,3 };
		m_loots.push_back(loot1);
		break;
	}
	case EnviroSpawnName::Stone:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 4;
		m_stats.m_minToolsRarity = Rarity::Iron;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack(ITEM_ROCK,1)), 2,5 };
		m_loots.push_back(loot1);
		break;
	}
	}
}
