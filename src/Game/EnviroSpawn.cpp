#include "pch.h"
#include "EnviroSpawn.h"
#include "EnviroProps.h"
#include "SpriteContainer.h"
#include "GameScene.h"
#include "GameData.h"
#include "FarmManager.h"

EnviroSpawn* EnviroSpawn::Get() {
	static EnviroSpawn instance;
	return &instance;
}

void EnviroSpawn::InitEnviroSpawn()
{
	for (int i = 0; i < (int)EnviroSpawnName::Count; i++) {
		EnviroInfo* enviro = new EnviroInfo;
		switch (i) {
		case 0:
			enviro->m_name = EnviroSpawnName::Tree;
			enviro->m_stats = { 1,3 };
			break;
		case 1:
			enviro->m_name = EnviroSpawnName::Shrub;
			enviro->m_stats = { 2,4 };
			break;
		case 2:
			enviro->m_name = EnviroSpawnName::Grass;
			enviro->m_stats = { 3,5 };
			break;
		case 3:
			enviro->m_name = EnviroSpawnName::Rock;
			enviro->m_stats = { 3,6 };
			break;
		case 4:
			enviro->m_name = EnviroSpawnName::Stone;
			enviro->m_stats = { 0,0 };
			break;
		case 5:
			enviro->m_name = EnviroSpawnName::IronOre;
			enviro->m_stats = { 2,5 };
			break;
		case 6:
			enviro->m_name = EnviroSpawnName::GoldOre;
			enviro->m_stats = { 1,4 };
			break;
		case 7:
			enviro->m_name = EnviroSpawnName::DiamondOre;
			enviro->m_stats = { 0,3 };
			break;
		default:
			break;
		}

		if (i < 5) {
			m_farmEnviroList.push_back(enviro);
		}
		if (i >= 3) {
			m_caveEnviroList.push_back(enviro);
		}
	}


	m_farmEnviroList[3]->m_stats = { 0,0 };
	UpdateFreeFarmTiles();

	NewGameEnviroSpawn();
}

void EnviroSpawn::SpawnEnviroProps()
{
	if (m_numOfEnviro >= m_maxTiles) {
		return;
	}


	for (int i = 0; i < m_farmEnviroList.size(); i++) {
		int spawnCount = Aleatory::GetInt(m_farmEnviroList[i]->m_stats.m_minSpawn, m_farmEnviroList[i]->m_stats.m_maxSpawn);
		for (int j = 0; j < spawnCount; j++) {
			CreateEnviroProps(m_farmEnviroList[i]->m_name, -1, SceneManager::Get()->GetScene("game"));
		}
	}

	for (int i = 0; i < m_caveEnviroList.size(); i++) {
		int spawnCount = Aleatory::GetInt(m_caveEnviroList[i]->m_stats.m_minSpawn, m_caveEnviroList[i]->m_stats.m_maxSpawn);
		for (int j = 0; j < spawnCount; j++) {
			CreateEnviroProps(m_caveEnviroList[i]->m_name, -1, SceneManager::Get()->GetScene("cave"));
		}
	}
}

int EnviroSpawn::GetEnviroDay()
{
	constexpr float IRL_SECONDS_PER_GAME_HOUR = 40.0f;
	constexpr float HOURS_PER_DAY = 24.f;
	constexpr float SIX_HOURS = 6.f * IRL_SECONDS_PER_GAME_HOUR;

	float secPerDay = HOURS_PER_DAY * IRL_SECONDS_PER_GAME_HOUR;
	float timeSpent = GameData::Get().GetTimeSpent();

	float shifted = timeSpent - SIX_HOURS;

	if (shifted < 0.f)
		shifted = 0.f;

	return static_cast<int>(shifted / secPerDay);
}

void EnviroSpawn::NewGameEnviroSpawn()
{
	for (int i = 0; i < 200; i++) {
		int tile = Aleatory::GetInt(0, m_freeFarmTiles.size() - 1);
		while (m_freeFarmTiles[tile]->GetPosition().GetX() >= 500) {
			tile = Aleatory::GetInt(0, m_freeFarmTiles.size() - 1);
		}
		CreateEnviroProps(m_farmEnviroList[Aleatory::GetInt(0, m_farmEnviroList.size() - 1)]->m_name, tile, SceneManager::Get()->GetScene("game"));
	}

}

void EnviroSpawn::UpdateFreeFarmTiles()
{
	m_freeFarmTiles.clear();
	std::vector<FarmTile*> farmTile = FarmManager::Get()->GetAllFarmTiles();
	for (int i = 0; i < farmTile.size(); i++) {
		if (farmTile[i]->CanGrow() == true) {
			m_freeFarmTiles.push_back(farmTile[i]);
		}
	}
	for (int j = 0; j < m_freeFarmTiles.size() / 2; j++) {
		std::swap(m_freeFarmTiles[j], m_freeFarmTiles[Aleatory::GetInt(0, m_freeFarmTiles.size() - 1)]);
	}
}

void EnviroSpawn::CreateEnviroProps(EnviroSpawnName _name, int _tile, Scene* _scene)
{
	EnviroProps* Props;
	if (_tile < 0) {
		_tile = Aleatory::GetInt(0, m_freeFarmTiles.size() - 1);
	}
	if (m_freeFarmTiles[_tile]->CanGrow() == false) {
		return;
	}
	Props = _scene->CreateEntity<EnviroProps>(m_freeFarmTiles[_tile]->GetPosition(), { 128,128 }, "", Shape::Rectangle);
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
		Props->AddSprite("default", "Oak_Tree", { 128, 128 }, 0, { 0, -28 });
		Props->GetCollider()->Resize({ 16,16 });
		break;
	case EnviroSpawnName::Shrub:
		Props->AddSprite("default", "Oak_Shrub", { 46, 68 }, 0, { 1,-18 });
		Props->GetCollider()->Resize({ 8,8 });
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
		Props->AddSprite("default", "Stone", { 32, 32 }, 0, { 0,-8 });
		Props->GetCollider()->Resize({ 32,16 });
		break;

	default:
		break;
	}

	if (Props->GetType() == EnviroSpawnName::Stone) {
		float radius = 23.f;
		float radiusSqr = radius * radius;
		int posCheck = 0;

		for (int tile = 0; tile < m_freeFarmTiles.size(); tile++) {

			float dx = m_freeFarmTiles[tile]->GetPosition().GetX() - m_freeFarmTiles[_tile]->GetPosition().GetX();
			float dy = m_freeFarmTiles[tile]->GetPosition().GetY() - m_freeFarmTiles[_tile]->GetPosition().GetY();

			if ((dx * dx + dy * dy) <= radiusSqr && m_freeFarmTiles[tile]->CanGrow() == true) {
				posCheck++;
			}
		}
		if (posCheck <= 9) {
			for (int pos = 0; pos < m_freeFarmTiles.size(); pos++) {
				if (m_freeFarmTiles[pos]->GetPosition().GetX() >= 500) continue;
				posCheck = 0;
				for (int tile = 0; tile < m_freeFarmTiles.size(); tile++) {

					float dx = m_freeFarmTiles[tile]->GetPosition().GetX() - m_freeFarmTiles[pos]->GetPosition().GetX();
					float dy = m_freeFarmTiles[tile]->GetPosition().GetY() - m_freeFarmTiles[pos]->GetPosition().GetY();

					if ((dx * dx + dy * dy) <= radiusSqr && m_freeFarmTiles[tile]->CanGrow() == true) {
						posCheck++;
					}
				}
				if (posCheck >= 9) {
					Props->SetFarmTile(m_freeFarmTiles[pos]);
					break;
				}
			}
		}
		else {
			Props->SetFarmTile(m_freeFarmTiles[_tile]);
		}

		if (Props->GetFarmTile() == NULL) {
			Props->Destroy();
			return;
		}

		for (int tile = 0; tile < m_freeFarmTiles.size(); tile++) {

			float dx = m_freeFarmTiles[tile]->GetPosition().GetX() - Props->GetFarmTile()->GetPosition().GetX();
			float dy = m_freeFarmTiles[tile]->GetPosition().GetY() - Props->GetFarmTile()->GetPosition().GetY();

			if ((dx * dx + dy * dy) <= radiusSqr) {
				m_freeFarmTiles[tile]->SetCanGrow(false);
			}
		}
	}

	else {
		Props->SetFarmTile(m_freeFarmTiles[_tile]);
		m_freeFarmTiles[_tile]->SetCanGrow(false);
	}

	UpdateFreeFarmTiles();
}

void EnviroSpawn::Update() {
	int curentDay = GetEnviroDay();

	if (curentDay == m_lastDay) return;

	if (m_lastDay != -1) {
		SpawnEnviroProps();
	}


	m_lastDay = curentDay;
}
