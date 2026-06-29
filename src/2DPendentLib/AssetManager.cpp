#include "pch.h"
#include "AssetManager.h"
#include "Utils.h"
#include "Render.h"
#include "json.hpp"

AssetManager* AssetManager::Get()
{
	static AssetManager instance;
	return &instance;
}

AssetManager::AssetManager()
{
	m_texturesMap.reserve(512);      // nombre de textures chargées en même temps
	m_texturesVector.reserve(512);   // cohérent avec la map
	m_textureIdsVector.reserve(512); // cohérent avec la map
}

AssetManager::~AssetManager()
{
	// Libérer les textures chargées
	for (SDL_Texture* texture : m_texturesVector)
	{
		SDL_DestroyTexture(texture);
	}
	m_texturesVector.clear();
	m_texturesMap.clear();
	m_textureIdsVector.clear();
}

void AssetManager::AddTextureByRepertory(const std::string& p_path)
{
	std::ifstream file(p_path);
	if (!file.is_open())
	{
		SDL_Log("Lib/AssetManager.cpp/AddTexture/failed to open file: %s", p_path.c_str());
		return;
	}

	nlohmann::ordered_json assets_json;
	file >> assets_json;

	for (auto& [categorie, assets] : assets_json.items())
	{
		for (auto& asset : assets)
		{
			std::string path = asset["path"].get<std::string>();
			std::string autoId = Utils::GetNameFromPath(path);
			auto it = m_texturesMap.find(autoId);
			if (it != m_texturesMap.end())
			{
				SDL_Log("Lib/AssetManager.cpp/AddTexture/GetNameFromPath/Texture already loaded with ID : %s", autoId.c_str());
				continue;
			}
			SDL_Texture* texture = IMG_LoadTexture(m_renderer, path.c_str());
			if (texture == nullptr)
			{
				SDL_Log("Lib/AssetManager.cpp/AddTexture/IMG_LoadTexture/failed to load texture: %s", SDL_GetError());
				continue;
			}
			m_texturesMap[autoId] = m_texturesVector.size();
			m_texturesVector.push_back(texture);
			m_textureIdsVector.push_back(autoId);
		}
	}
}

void AssetManager::AddTextureByPath(const std::string& p_path)
{
	std::string autoId = Utils::GetNameFromPath(p_path);
	auto it = m_texturesMap.find(autoId);
	if (it != m_texturesMap.end())
	{
		SDL_Log("Lib/AssetManager.cpp/AddTextureByPath/GetNameFromPath/Texture already loaded with ID : %s", autoId.c_str());
		return;
	}
	SDL_Texture* texture = IMG_LoadTexture(m_renderer, p_path.c_str());
	if (texture == nullptr)
	{
		SDL_Log("Lib/AssetManager.cpp/AddTextureByPath/IMG_LoadTexture/failed to load texture: %s", SDL_GetError());
		return;
	}
	m_texturesMap[autoId] = m_texturesVector.size();
	m_texturesVector.push_back(texture);
	m_textureIdsVector.push_back(autoId);
}

void AssetManager::DestroyTexture(const std::string& p_id)
{
	auto it = m_texturesMap.find(p_id);
	if (it == m_texturesMap.end())
	{
		SDL_Log("Lib/AssetManager.cpp/DestroyTexture/find/Texture with id '%s' not found", p_id.c_str());
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_texturesVector.size() - 1;

	SDL_DestroyTexture(m_texturesVector[indexToDelete]);

	if (indexToDelete != lastIndex)
	{
		m_texturesVector[indexToDelete] = m_texturesVector[lastIndex]; 
		m_textureIdsVector[indexToDelete] = m_textureIdsVector[lastIndex];
		m_texturesMap[m_textureIdsVector[indexToDelete]] = indexToDelete;
	}

	m_texturesVector.pop_back();
	m_textureIdsVector.pop_back();
	m_texturesMap.erase(it);
}

void AssetManager::ClearTextures()
{
	for (SDL_Texture* texture : m_texturesVector)
	{
		SDL_DestroyTexture(texture);
	}
	m_texturesVector.clear();
	m_texturesMap.clear();
	m_textureIdsVector.clear();
}

SDL_Texture* AssetManager::GetTexture(const std::string& p_id) const
{
	auto it = m_texturesMap.find(p_id);
	if (it == m_texturesMap.end())
	{
		SDL_Log("Lib/AssetManager.cpp/GetTexture/find/Texture with id '%s' not found", p_id.c_str());
		return nullptr;
	}
	else if (it != m_texturesMap.end())
	{
		return m_texturesVector[it->second];
	}
}
