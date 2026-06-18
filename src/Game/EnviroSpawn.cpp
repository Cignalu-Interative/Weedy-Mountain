#include "pch.h"
#include "EnviroSpawn.h"
#include "EnviroProps.h"
#include "SpriteContainer.h"
#include "GameScene.h"

void EnviroSpawn::InitEnviroSpawn()
{
	for (int i = 0; i < (int)EnviroSpawnName::Count; i++) {
		EnviroInfo* Enviro = new EnviroInfo;
		switch (i) {
		case 0:
			Enviro->m_name = EnviroSpawnName::Tree;
			Enviro->m_stats = { 1,3 };
			break;
		case 1:
			Enviro->m_name = EnviroSpawnName::Shrub;
			Enviro->m_stats = { 2,4 };
			break;
		case 2:
			Enviro->m_name = EnviroSpawnName::Grass;
			Enviro->m_stats = { 3,5 };
			break;
		case 3:
			Enviro->m_name = EnviroSpawnName::Rock;
			Enviro->m_stats = { 0,0 };
			break;
		case 4:
			Enviro->m_name = EnviroSpawnName::Stone;
			Enviro->m_stats = {0,0 };
			break;
		default:
			break;
		}

		m_enviroList.push_back(Enviro);
	}
}

void EnviroSpawn::SpawnEnviroProps()
{
	if (m_numOfEnviro >= m_maxTiles) {
		return;
	}

	for (int i = 0; i < m_enviroList.size(); i++) {

		int spawnCount = Aleatory::GetInt(m_enviroList[i]->m_stats.m_minSpawn, m_enviroList[i]->m_stats.m_maxSpawn);

		for (int j = 0; j < spawnCount; j++) {
			CreateEnviroProps(m_enviroList[i]->m_name);
		}
	}
}

void EnviroSpawn::CreateEnviroProps(EnviroSpawnName _name)
{
	Scene* gs = SceneManager::Get()->GetScene("game");
	EnviroProps* Props;
	Props = gs->CreateEntity<EnviroProps>({ Aleatory::GetFloat(0.f, 200.f), Aleatory::GetFloat(0.f, 200.f) }, { 128,128 }, "", Shape::Rectangle);
	Props->RemoveSprite("default");
	Props->SetType(_name);
	Props->SetTag(2);
	Props->ShowCollider(true);
	Props->GetCollider()->SetLayer(ENVIRO);
	Props->GetCollider()->SetMask(PLAYER);
	Props->InitialisePhysicsBody(ShapeBodyPhysics::Static);

	switch (_name)
	{
	case EnviroSpawnName::Tree:
		Props->AddSprite("default", "Oak_Tree", {128, 128}, 0);
		Props->GetCollider()->Resize({ 16,16 });
		Props->GetCollider()->SetOffset({ 0,28 });
		break;
	case EnviroSpawnName::Shrub:
		Props->AddSprite("default", "Oak_Shrub", { 46, 68 }, 0);
		Props->GetCollider()->Resize({ 8,8 });
		Props->GetCollider()->SetOffset({ 0,18 });
		break;
	case EnviroSpawnName::Grass:
		Props->AddSprite("default", "Grass", { 18, 18 }, 0);
		Props->GetCollider()->SetIsTrigger(true);
		Props->GetCollider()->Resize({ 16,16 });
		break;
	case EnviroSpawnName::Rock:
		Props->AddSprite("default", "Rock", { 18, 18 }, 0);
		Props->GetCollider()->Resize({ 16,16 });
		break;
	case EnviroSpawnName::Stone:
		Props->AddSprite("default", "Stone", { 32, 32 }, 0);
		Props->GetCollider()->Resize({ 32,16 });
		Props->GetCollider()->SetOffset({ 0,8 });
		break;
	default:
		break;
	}
}
