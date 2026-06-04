#include "pch.h"
#include "EnviroSpawn.h"
#include "EnviroProps.h"



void EnviroSpawn::InitEnviroSpawn()
{
	EnviroInfo* Enviro = new EnviroInfo;
	for (int i = 0; i < (int)EnviroSpawnName::Count; i++) {
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
			Enviro->m_stats = { 0,0 };
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
			//SceneManager::Get()->GetCurrentScene()->CreateEntity<EnviroProps>(/*position aléatoire*/, /*dimension*/, /*textureID*/, Shape::Rectangle)->SetType(m_enviroList[i]->m_name);
		}
	}
}