#include "pch.h"
#include "EnviroSpawn.h"
#include "EnviroProps.h"
#include "SpriteContainer.h"
#include "GameScene.h"
#include "GameData.h"
#include "FarmManager.h"
#include "CaveManager.h"
#include <variant>

#include "SaveManager.h"

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
			enviro->m_stats = { 40,50 };
			break;
		case 4:
			enviro->m_name = EnviroSpawnName::Stone;
			enviro->m_stats = { 0,0 };
			break;
		case 5:
			enviro->m_name = EnviroSpawnName::IronOre;
			enviro->m_stats = { 1,4 };
			break;
		case 6:
			enviro->m_name = EnviroSpawnName::GoldOre;
			enviro->m_stats = { 0,3 };
			break;
		case 7:
			enviro->m_name = EnviroSpawnName::DiamondOre;
			enviro->m_stats = { 0,2 };
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

	UpdateFreeTiles();

	NewGameEnviroSpawn();

	SaveManager::Get().RegisterProvider(this);
}

void EnviroSpawn::SpawnEnviroProps()
{
	if (m_numOfEnviro >= m_maxTiles) {
		return;
	}

	UpdateFreeTiles();

	switch (GameData::Get().GetCurrentScene()) {
	case CurrentScene::FARM:
		m_caveDayCounter++;

		for (int i = 0; i < m_farmEnviroList.size(); i++) {
			int spawnCount = Aleatory::GetInt(m_farmEnviroList[i]->m_stats.m_minSpawn, m_farmEnviroList[i]->m_stats.m_maxSpawn);
			for (int j = 0; j < spawnCount; j++) {
				CreateEnviroProps(m_farmEnviroList[i]->m_name, -1, SceneManager::Get()->GetScene("game"));
			}
		}
		break;
	case CurrentScene::CAVE:
		m_farmDayCounter++;

		for (int i = 0; i < m_caveEnviroList.size(); i++) {
			int spawnCount = Aleatory::GetInt(m_caveEnviroList[i]->m_stats.m_minSpawn, m_caveEnviroList[i]->m_stats.m_maxSpawn);
			for (int j = 0; j < spawnCount; j++) {
				CreateEnviroProps(m_caveEnviroList[i]->m_name, -1, SceneManager::Get()->GetScene("cave"));
			}
		}
		break;
	default:
		m_caveDayCounter++;
		m_farmDayCounter++;
		break;
	
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
	UpdateFreeTiles();

	if (m_freeTiles.empty()) // Evite un crash du vector freetiles car vide et donc size - 1 fait planter le programme
	{
		return;
	}

	for (int i = 0; i < 150; i++) {
		int tile = Aleatory::GetInt(0, m_freeTiles.size() - 1);
		while (TileUtils::GetPosition(m_freeTiles[tile]).GetX() >= 500) {
			tile = Aleatory::GetInt(0, m_freeTiles.size() - 1);
		}
		CreateEnviroProps(m_farmEnviroList[Aleatory::GetInt(0, m_farmEnviroList.size() - 1)]->m_name, tile, SceneManager::Get()->GetScene("game"));
	}

	m_farmEnviroList.erase(m_farmEnviroList.begin() + 3, m_farmEnviroList.end());
}

void EnviroSpawn::UpdateFreeTiles()
{
	m_freeTiles.clear();
	std::vector<FarmTile*> farmTile;
	std::vector<CaveTile*> caveTile;
	switch (GameData::Get().GetCurrentScene()) {
	case CurrentScene::FARM:
		farmTile = FarmManager::Get()->GetAllFarmTiles();
		for (int i = 0; i < farmTile.size(); i++) {
			if (farmTile[i]->CanGrow() == true) {
				m_freeTiles.push_back(farmTile[i]);
			}
		}
		for (int j = 0; j < m_freeTiles.size() / 2; j++) {
			std::swap(m_freeTiles[j], m_freeTiles[Aleatory::GetInt(0, m_freeTiles.size() - 1)]);
		}
		break;
	case CurrentScene::CAVE:
		caveTile = CaveManager::Get()->GetAllCaveTiles();
		for (int i = 0; i < caveTile.size(); i++) {
			if (caveTile[i]->CanGrow() == true) {
				m_freeTiles.push_back(caveTile[i]);
			}
		}
		for (int j = 0; j < m_freeTiles.size() / 2; j++) {
			std::swap(m_freeTiles[j], m_freeTiles[Aleatory::GetInt(0, m_freeTiles.size() - 1)]);
		}
		break;
	default:
		break;
	}
}

Tile EnviroSpawn::FindBestStoneTile(const Tile& p_initialTile) const
{
	constexpr float radius = 23.f;
	constexpr float radiusSqr = radius * radius;
	constexpr int minNeighbors = 9;

	auto countFreeNeighbors = [&](const Tile& p_center) -> int
		{
			Vector2f center = TileUtils::GetPosition(p_center);
			int count = 0;

			for (const Tile& tile : m_freeTiles)
			{
				Vector2f p = TileUtils::GetPosition(tile);
				float dx = p.GetX() - center.GetX();
				float dy = p.GetY() - center.GetY();

				if ((dx * dx + dy * dy) <= radiusSqr && TileUtils::CanGrow(tile))
					count++;
			}
			return count;
		};

	if (countFreeNeighbors(p_initialTile) > minNeighbors)
		return p_initialTile;

	for (const Tile& candidate : m_freeTiles)
	{
		if (TileUtils::GetPosition(candidate).GetX() >= 500)
			continue;

		if (countFreeNeighbors(candidate) >= minNeighbors)
			return candidate;
	}

	return {};
}

void EnviroSpawn::CreateEnviroProps(EnviroSpawnName _name, int _tile, Scene* _scene)
{
	UpdateFreeTiles();

	if (m_freeTiles.empty())
		return;

	if (_tile < 0)
		_tile = Aleatory::GetInt(0, m_freeTiles.size() - 1);

	if (TileUtils::CanGrow(m_freeTiles[_tile]) == false)
		return;

	Tile targetTile = m_freeTiles[_tile];

	if (_name == EnviroSpawnName::Stone)
	{
		targetTile = FindBestStoneTile(m_freeTiles[_tile]);

		if (TileUtils::IsSet(targetTile) == false)
			return; 
	}

	CreateEnviroPropsAt(_name, targetTile, _scene);

	UpdateFreeTiles();
}

EnviroProps* EnviroSpawn::CreateEnviroPropsAt(EnviroSpawnName p_name, const Tile& p_tile, Scene* p_scene)
{
	if (p_scene == nullptr || TileUtils::IsSet(p_tile) == false)
		return nullptr;

	EnviroProps* Props = p_scene->CreateEntity<EnviroProps>(TileUtils::GetPosition(p_tile), { 128,128 }, "", Shape::Rectangle);

	Props->RemoveSprite("default");
	Props->SetType(p_name);
	Props->SetTag(2);
	Props->ShowCollider(true);
	Props->GetCollider()->SetLayer(ENVIRO);
	Props->GetCollider()->SetMask(PLAYER);
	Props->InitialisePhysicsBody(ShapeBodyPhysics::Static);

	ConfigureEnviroPropsVisual(Props, p_name);

	Props->SetTile(p_tile);
	MarkTileOccupied(p_tile, p_name);

	RegisterProps(Props);

	return Props;
}

void EnviroSpawn::ConfigureEnviroPropsVisual(EnviroProps* Props, EnviroSpawnName _name)
{
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
	case EnviroSpawnName::IronOre:
		Props->AddSprite("default", "Iron_Ore", { 18, 18 }, 0);
		Props->GetCollider()->Resize({ 16,16 });
		break;
	case EnviroSpawnName::GoldOre:
		Props->AddSprite("default", "Gold_Ore", { 18, 18 }, 0);
		Props->GetCollider()->Resize({ 16,16 });
		break;
	case EnviroSpawnName::DiamondOre:
		Props->AddSprite("default", "Diamond_Ore", { 18, 18 }, 0);
		Props->GetCollider()->Resize({ 16,16 });
		break;
	default:
		break;
	}
}

void EnviroSpawn::MarkTileOccupied(const Tile& p_tile, EnviroSpawnName p_name)
{
	if (p_name != EnviroSpawnName::Stone)
	{
		TileUtils::SetCanGrow(p_tile, false);
		return;
	}

	float radius = 23.f;
	float radiusSqr = radius * radius;
	Vector2f center = TileUtils::GetPosition(p_tile);

	bool isCave = std::holds_alternative<CaveTile*>(p_tile);

	if (isCave)
	{
		for (CaveTile* t : CaveManager::Get()->GetAllCaveTiles())
		{
			Vector2f p = t->GetPosition();
			float dx = p.GetX() - center.GetX();
			float dy = p.GetY() - center.GetY();

			if ((dx * dx + dy * dy) <= radiusSqr)
				t->SetCanGrow(false);
		}
	}
	else
	{
		for (FarmTile* t : FarmManager::Get()->GetAllFarmTiles())
		{
			Vector2f p = t->GetPosition();
			float dx = p.GetX() - center.GetX();
			float dy = p.GetY() - center.GetY();

			if ((dx * dx + dy * dy) <= radiusSqr)
				t->SetCanGrow(false);
		}
	}
}

void EnviroSpawn::Update() {
	int curentDay = GetEnviroDay();

	UpdateFreeTiles();

	switch (GameData::Get().GetCurrentScene()) {
	case CurrentScene::FARM:
		while (m_farmDayCounter > 0) {
			for (int i = 0; i < m_farmEnviroList.size(); i++) {
				int spawnCount = Aleatory::GetInt(m_farmEnviroList[i]->m_stats.m_minSpawn, m_farmEnviroList[i]->m_stats.m_maxSpawn);
				for (int j = 0; j < spawnCount; j++) {
					CreateEnviroProps(m_farmEnviroList[i]->m_name, -1, SceneManager::Get()->GetScene("game"));
				}
			}
			m_farmDayCounter--;
		}
		break;
	case CurrentScene::CAVE:
		while (m_caveDayCounter > 0) {
			for (int i = 0; i < m_caveEnviroList.size(); i++) {
				int spawnCount = Aleatory::GetInt(m_caveEnviroList[i]->m_stats.m_minSpawn, m_caveEnviroList[i]->m_stats.m_maxSpawn);
				for (int j = 0; j < spawnCount; j++) {
					CreateEnviroProps(m_caveEnviroList[i]->m_name, -1, SceneManager::Get()->GetScene("cave"));
				}
			}
			m_caveDayCounter--;
		}
		break;
	default:
		break;
	}

	if (curentDay == m_lastDay) return;

	if (m_lastDay != -1) {
		SpawnEnviroProps();
	}


	m_lastDay = curentDay;
}

void EnviroSpawn::RegisterProps(EnviroProps* p_props)
{
	if (p_props == nullptr) 
		return;

	auto it = std::find(m_liveEnviroProps.begin(), m_liveEnviroProps.end(), p_props);

	if (it != m_liveEnviroProps.end()) 
		return;
	m_liveEnviroProps.push_back(p_props);
}

void EnviroSpawn::UnregisterProps(EnviroProps* p_props)
{
	auto it = std::find(m_liveEnviroProps.begin(), m_liveEnviroProps.end(), p_props);

	if (it != m_liveEnviroProps.end())
		m_liveEnviroProps.erase(it);
}

void EnviroSpawn::Save(nlohmann::json& out)
{
	json arr = json::array();

	for (EnviroProps* props : m_liveEnviroProps)
	{
		const Tile& tile = props->GetTile();

		if (TileUtils::IsSet(tile) == false)
			continue;

		bool isCave = std::holds_alternative<CaveTile*>(tile); 

		arr.push_back({
			{"type", (int)props->GetType()},
			{"isCave", isCave},
			{"posX", TileUtils::GetPosition(tile).GetX()},
			{"posY", TileUtils::GetPosition(tile).GetY()},
			{"hitLeft", props->GetStats()->m_hitToDestroy}
			});
	}

	out["props"] = arr;
	out["lastDay"] = m_lastDay;
	out["farmDayCounter"] = m_farmDayCounter;
	out["caveDayCounter"] = m_caveDayCounter;
}

void EnviroSpawn::Load(const nlohmann::json& in)
{
	Scene* farmScene = SceneManager::Get()->GetScene("game");
	Scene* caveScene = SceneManager::Get()->GetScene("cave");

	std::vector<EnviroProps*> toDestroy = m_liveEnviroProps;
	for (EnviroProps* props : toDestroy)
	{
		props->DestroyWithoutLoot();
	}
	m_liveEnviroProps.clear();

	for (auto& p : in.value("props", json::array()))
	{
		EnviroSpawnName type = (EnviroSpawnName)p.value("type", 0);
		bool isCave = p.value("isCave", false);
		Vector2f pos = { p.value("posX", 0.f), p.value("posY", 0.f) };

		Tile tile;
		Scene* targetScene = nullptr;

		if (isCave)
		{
			if (caveScene == nullptr) 
				continue;

			CaveTile* ct = CaveManager::Get()->GetNearestTile(pos); 

			if (ct == nullptr)
				continue;

			tile = ct;
			targetScene = caveScene;
		}
		else
		{
			if (farmScene == nullptr) 
				continue;

			FarmTile* ft = FarmManager::Get()->GetNearestTile(pos);

			if (ft == nullptr) 
				continue;

			tile = ft;
			targetScene = farmScene;
		}

		EnviroProps* props = CreateEnviroPropsAt(type, tile, targetScene);

		if (props)
			props->GetStats()->m_hitToDestroy = p.value("hitLeft", 1);
	}

	m_lastDay = in.value("lastDay", -1);
	m_farmDayCounter = in.value("farmDayCounter", 0);
	m_caveDayCounter = in.value("caveDayCounter", 0);
}