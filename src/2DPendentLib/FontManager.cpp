#include "pch.h"
#include "FontManager.h"
#include "json.hpp"
#include "Utils.h"

FontManager* FontManager::Get()
{
	static FontManager instance;
	return &instance;
}

FontManager::FontManager()
{
	m_fontsMap.reserve(10);      // nombre de fonts chargées en même temps
	m_fontsVector.reserve(10);   // cohérent avec la map
	m_fontIdsVector.reserve(10); // cohérent avec la map
}

FontManager::~FontManager()
{
	ClearFonts();
}

void FontManager::AddFontByRepertory(const std::string& p_path)
{
	std::ifstream file(p_path);
	if (!file.is_open())
	{
		//SDL_Log("Lib/FontManager.cpp/AddFontByRepertory/failed to open file: %s", p_path.c_str());
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
			auto it = m_fontsMap.find(autoId);
			if (it != m_fontsMap.end())
			{
				//SDL_Log("Lib/FontManager.cpp/AddFont/GetNameFromPath/Font already loaded with ID : %s", autoId.c_str());
				continue;
			}
			m_fontsMap[autoId] = m_fontsVector.size();
			m_fontsVector.push_back(std::move(path));
			m_fontIdsVector.push_back(autoId);
		}
	}
}

void FontManager::AddFontByPath(const std::string& p_path)
{
	std::string autoId = Utils::GetNameFromPath(p_path);
	auto it = m_fontsMap.find(autoId);
	if (it != m_fontsMap.end())
	{
		//SDL_Log("Lib/FontManager.cpp/AddFontByPath/GetNameFromPath/Font already loaded with ID : %s", autoId.c_str());
		return;
	}
	m_fontsMap[autoId] = m_fontsVector.size();
	m_fontsVector.push_back(p_path);
	m_fontIdsVector.push_back(autoId);
}

void FontManager::DestroyFont(const std::string& p_id)
{
	auto it = m_fontsMap.find(p_id);
	if (it == m_fontsMap.end())
	{
		//SDL_Log("Lib/FontManager.cpp/DestroyFont/find/Font with id '%s' not found", p_id.c_str());
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_fontsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		m_fontsVector[indexToDelete] = m_fontsVector[lastIndex];
		m_fontIdsVector[indexToDelete] = m_fontIdsVector[lastIndex];
		m_fontsMap[m_fontIdsVector[indexToDelete]] = indexToDelete;
	}

	m_fontsVector.pop_back();
	m_fontIdsVector.pop_back();
	m_fontsMap.erase(it);
}

void FontManager::ClearFonts()
{
	m_fontsVector.clear();
	m_fontsMap.clear();
	m_fontIdsVector.clear();
}

const std::string& FontManager::GetFont(const std::string& p_id) const
{
	auto it = m_fontsMap.find(p_id);
	if (it == m_fontsMap.end())
	{
		//SDL_Log("Lib/FontManager.cpp/GetFont/find/Font with id '%s' not found", p_id.c_str());
		static std::string emptyString;
		return emptyString;
	}
	else if (it != m_fontsMap.end())
	{
		return m_fontsVector[it->second];
	}
}