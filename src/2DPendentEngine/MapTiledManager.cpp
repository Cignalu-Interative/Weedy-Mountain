#include "pch.h"
#include "MapTiledManager.h"

MapTiledManager::MapTiledManager()
{
	m_maps.reserve(16); // Réserver de la mémoire pour 16 maps afin d'éviter les reallocations fréquentes lors de l'ajout de maps
	m_mapsVector.reserve(16); // Réserver de la mémoire pour 16 maps afin d'éviter les reallocations fréquentes lors de l'ajout de maps
	m_mapsIdsVector.reserve(16); // Réserver de la mémoire pour 16 maps afin d'éviter les reallocations fréquentes lors de l'ajout de maps
}

MapTiledManager::~MapTiledManager()
{
	// Libérer les maps chargées
	for (MapTiled* map : m_mapsVector)
	{
		delete map;
	}
	m_mapsVector.clear();
	m_maps.clear();
	m_mapsIdsVector.clear();
}

MapTiledManager* MapTiledManager::Get()
{
	static MapTiledManager instance;
	return &instance;
}

void MapTiledManager::AddMaps(const std::string& p_pathMaps)
{
	std::ifstream file(p_pathMaps);
	if (!file.is_open())
	{
		std::cerr << "Engine/MapTiledManager.cpp/AddMaps/is_open/ error not open" << std::endl;
	}

	nlohmann::ordered_json maps_json;
	file >> maps_json;

	for (auto& MapsRepertory_data : maps_json["maps"])
	{
		m_infoMapToLoad.s_MapPath = MapsRepertory_data["path"];
		m_infoMapToLoad.s_MapName = Utils::GetNameFromPath(m_infoMapToLoad.s_MapPath);

		if (m_maps.find(m_infoMapToLoad.s_MapName) != m_maps.end())
		{
			// La map est déjà chargée, ignorer le chargement
			std::cerr << "MapTiledManager.cpp/AddMaps/GetNameFromPath/Map already loaded with name : " << m_infoMapToLoad.s_MapName << std::endl;
			continue;
		}

		MapTiled* map = new MapTiled(m_infoMapToLoad.s_MapPath, m_render);
		m_maps[m_infoMapToLoad.s_MapName] = m_mapsVector.size();
		m_mapsVector.push_back(map);
		m_mapsIdsVector.push_back(m_infoMapToLoad.s_MapName);

		if (m_maps.size() == 1)
		{
			SetCurrentMap(m_infoMapToLoad.s_MapName);
		}
		std::cerr << "Map Charger : " << m_infoMapToLoad.s_MapName << std::endl;
	}
}

void MapTiledManager::DestroyMaps(const std::string& p_nameMaps)
{
	auto it = m_maps.find(p_nameMaps);
	if (it == m_maps.end())
	{
		std::cerr << "Engine/MapTiledManager.cpp/DestroyMaps/find/Map with name '" << p_nameMaps << "' not found" << std::endl;
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_mapsVector.size() - 1;

	delete m_mapsVector[indexToDelete];

	if (indexToDelete != lastIndex)
	{
		m_mapsVector[indexToDelete] = m_mapsVector[lastIndex];
		m_mapsIdsVector[indexToDelete] = m_mapsIdsVector[lastIndex];
		m_maps[m_mapsIdsVector[indexToDelete]] = indexToDelete;
	}

	m_mapsVector.pop_back();
	m_mapsIdsVector.pop_back();
	m_maps.erase(it);
}

void MapTiledManager::SetCurrentMap(const std::string& p_nameMaps)
{
	auto it = m_maps.find(p_nameMaps);
	if (it == m_maps.end())
	{
		std::cerr << "Engine/MapTiledManager.cpp/SetCurrentMap/ error not found" << std::endl;
		return;
	}
	m_current_map = m_mapsVector[it->second];
}

void MapTiledManager::DrawCurrentMap(Render* f_render, Window* f_window, Camera* p_camera)
{
	if (m_current_map == nullptr)
	{
		std::cerr << "Engine/MapTiledManager.cpp/DrawCurrentMap/ error no map to draw" << std::endl;
		return;
	}
	m_current_map->Draw(f_render, f_window, p_camera);
}