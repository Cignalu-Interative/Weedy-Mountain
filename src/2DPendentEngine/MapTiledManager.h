#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "2DPendentLib/MapTiled.h"
#include "2DPendentLib/tinyxml2.h"
#include "2DPendentLib/json.hpp"

class MapTiledManager
{
private:

	struct InfoMapToLoad
	{
		std::string s_MapName; // Nom de la map
		std::string s_MapPath; // Chemin vers le fichier de la map
	};
	InfoMapToLoad m_infoMapToLoad;

	std::unordered_map<std::string, size_t> m_maps;
	std::vector<MapTiled*> m_mapsVector; // Vector pour stocker les pointeurs des maps
	std::vector<std::string> m_mapsIdsVector;

	MapTiled* m_current_map = nullptr;

	Render* m_render = nullptr;

public:

	MapTiledManager();
	~MapTiledManager();

	static MapTiledManager* Get();

	void SetRender(Render* p_Render) { m_render = p_Render; }
	Render* GetRender() const { return m_render; }

	void AddMaps(const std::string& p_pathMaps);
	void DestroyMaps(const std::string& p_nameMaps);

	void SetCurrentMap(const std::string& p_nameMaps);
	MapTiled* GetCurrentMap() const { return m_current_map; }

	void DrawCurrentMap(Render* f_render, Window* f_window, Camera* p_camera);
};