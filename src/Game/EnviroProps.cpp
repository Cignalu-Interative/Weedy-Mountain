#include "pch.h"
#include "EnviroProps.h"
#include "Collision.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"
#include "SpriteContainer.h"
#include "ItemOnGround.h"
#include "ItemDataBase.h"
#include "FarmManager.h"


void EnviroProps::OnInitialize()
{

}

void EnviroProps::OnEvent()
{
	Vector2f playerPos = SceneManager::Get()->GetScene("game")->GetEntityByTag(1)->GetPosition();
	int playerLayer = SceneManager::Get()->GetScene("game")->GetEntityByTag(1)->GetDrawLayer();

	if (playerPos.GetX() > GetPosition().GetX() - 50 && playerPos.GetX() < GetPosition().GetX() + 50 && playerLayer < GetDrawLayer() && playerPos.GetY() > GetPosition().GetY() - GetSprite()->GetDimension().GetY()/2) {
		if(Sprite* sprite = GetSprite("default"))sprite->SetOpacity(100);
	}
	else {
		if (Sprite* sprite = GetSprite("default"))sprite->SetOpacity(255);
	}

}

void EnviroProps::OnUpdate()
{
	if (m_stats.m_hitToDestroy <= 0) {
		Destroy();
	}

	if (TileUtils::IsSet(m_stats.m_position)) {
		SetPosition(TileUtils::GetPosition(m_stats.m_position));
	}
	SetDrawLayer(200 + GetCollider()->GetPosition({ 1.f,1.f }).GetY());
}

void EnviroProps::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side)
{
}

void EnviroProps::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side)
{
}

void EnviroProps::OnDestroy()
{
	for (int i = 0; i < m_loots.size(); i++) {

		int numLoots = Aleatory::GetInt(m_loots[i]->m_minDrop, m_loots[i]->m_maxDrop);

		for (int j = 0; j < numLoots; j++) {
			CreateItem(i);
		}
	}

	if (!TileUtils::IsSet(m_stats.m_position)) {
		return;
	}
	TileUtils::SetCanGrow(m_stats.m_position, true);

	if (m_name == EnviroSpawnName::Stone) {
		std::vector<FarmTile*> m_farmTiles = FarmManager::Get()->GetAllFarmTiles();
		float radius = 23.f;
		float radiusSqr = radius * radius;
		Vector2f tilePos = TileUtils::GetPosition(m_stats.m_position);

		for (int tile = 0; tile < m_farmTiles.size(); tile++) {
			float dx = m_farmTiles[tile]->GetPosition().GetX() - tilePos.GetX();
			float dy = m_farmTiles[tile]->GetPosition().GetY() - tilePos.GetY();
			
			if ((dx * dx + dy * dy) <= radiusSqr) {
				m_farmTiles[tile]->SetCanGrow(true);
			}
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
	Loot = sc->CreateEntity<ItemOnGround>({ Aleatory::GetFloat(posX-25,posX + 25),Aleatory::GetFloat(posY - 10, posY + 10) }, { 16,16 }, path, Shape::Circle);
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
		m_stats.m_minToolsRarity = ToolRarity::Stone;
		m_stats.m_toolType = { ToolType::Axe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_WOOD",1)), 3,4 };
		EnviroLoots* loot2 = new EnviroLoots{ (ItemStack("ITEM_FIBER",1)), 1,2 };
		m_loots.push_back(loot1);
		m_loots.push_back(loot2);
		break;
	}
	case EnviroSpawnName::Shrub:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = ToolRarity::Wooden;
		m_stats.m_toolType = { ToolType::Axe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_WOOD",1)), 1,2 };
		EnviroLoots* Loot2 = new EnviroLoots{ (ItemStack("ITEM_FIBER",1)), 0,2 };
		m_loots.push_back(loot1);
		m_loots.push_back(Loot2);
		break;
	}
	case EnviroSpawnName::Grass:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 1;
		m_stats.m_minToolsRarity = ToolRarity::Wooden;
		m_stats.m_toolType = { ToolType::Hoe, ToolType::Spade };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_FIBER",1)), 1,3 };
		m_loots.push_back(loot1);
		break;
	}
	case EnviroSpawnName::Rock:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = ToolRarity::Wooden;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_ROCK",1)), 1,3 };
		m_loots.push_back(loot1);
		break;
	}
	case EnviroSpawnName::Stone:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 4;
		m_stats.m_minToolsRarity = ToolRarity::Iron;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_ROCK",1)), 2,5 };
		m_loots.push_back(loot1);
		break;
	}
	case EnviroSpawnName::IronOre:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = Rarity::Wooden;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_RAWIRON",1)), 1,3 };
		m_loots.push_back(loot1);
		break;

	}
	case EnviroSpawnName::GoldOre:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = Rarity::Wooden;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_RAWGOLD",1)), 1,3 };
		m_loots.push_back(loot1);
		break;
	}
	case EnviroSpawnName::DiamondOre:
	{
		m_name = p_name;
		m_stats.m_hitToDestroy = 2;
		m_stats.m_minToolsRarity = Rarity::Wooden;
		m_stats.m_toolType = { ToolType::Pickaxe };
		EnviroLoots* loot1 = new EnviroLoots{ (ItemStack("ITEM_DIAMOND",1)), 1,3 };
		m_loots.push_back(loot1);
		break;
	}
	default:
		break;
	}
}

bool EnviroProps::CanBeHitBy(ToolType toolType, ToolRarity toolRarity)
{
	bool typeMatches = false;
	for (ToolType accepted : m_stats.m_toolType)
	{
		if (accepted == toolType)
		{
			typeMatches = true;
			break;
		}
	}

	if (typeMatches == false)
		return false;

	if (static_cast<int>(toolRarity) < static_cast<int>(m_stats.m_minToolsRarity))
		return false;

	return true;
}

bool EnviroProps::TryHit(ToolType toolType, ToolRarity toolRarity)
{
	if (CanBeHitBy(toolType, toolRarity) == false)
		return false;

	m_stats.m_hitToDestroy--;
	return true;
}