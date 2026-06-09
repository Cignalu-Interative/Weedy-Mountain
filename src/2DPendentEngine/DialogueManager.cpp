#include "pch.h"
#include "DialogueManager.h"
#include "json.hpp"
#include "Utils.h"
#include <regex>

void DialogueManager::ParserDialogueFile(const std::string& p_path)
{
	std::ifstream file(p_path);
	if (!file.is_open())
	{
		std::cout << "Engine/DialogueManager.cpp/ParserDialogueFile/failed to open file: %s" << p_path.c_str() << std::endl;
		return;
	}

	std::string line; // pour iterer dans chaque ligne du fichier

	while (std::getline(file, line))
	{
		std::regex id(R"(@(\d+))");
		std::smatch match; // permet de stocker les valeurs demander par le regex
		
		DialogNode node;

		if (std::regex_search(line, match, id))
			node.s_id = std::stoi(match[1]);



	}
}

DialogueManager* DialogueManager::Get()
{
	static DialogueManager instance;
	return &instance;
}

DialogueManager::DialogueManager()
{
}

DialogueManager::~DialogueManager()
{
	//m_dialoguesMap.clear();
}

void DialogueManager::AddDialogueByRepertory(const std::string& p_path)
{
	//std::ifstream file(p_path);
	//if (!file.is_open())
	//{
	//	std::cout << "Lib/DialogueManager.cpp/AddDialogueByRepertory/failed to open file: %s" << p_path.c_str() << std::endl;
	//	return;
	//}

	//nlohmann::ordered_json assets_json;
	//file >> assets_json;

	//for (auto& [categorie, assets] : assets_json.items())
	//{
	//	for (auto& asset : assets)
	//	{
	//		std::string path = asset["path"].get<std::string>();
	//		std::string autoId = Utils::GetNameFromPath(path);
	//		auto it = m_dialoguesMap.find(autoId);
	//		if (it != m_dialoguesMap.end())
	//		{
	//			std::cout << "Lib/AssetManager.cpp/AddTexture/GetNameFromPath/Texture already loaded with ID : %s" << autoId.c_str() << std::endl;
	//			continue;
	//		}



	//		m_dialoguesMap[autoId] = m_dialoguesMap.size();
	//		//m_dialoguesMap.push_back(texture);
	//		m_dialoguesIdsVector.push_back(autoId);
	//	}
	//}
}

void DialogueManager::AddDialogueByPath(const std::string& p_path)
{
}

std::string* DialogueManager::GetDialogue(const std::string& p_name)
{
	return nullptr;
}

//void AssetManager::AddTextureByPath(const std::string& p_path)
//{
//	std::string autoId = Utils::GetNameFromPath(p_path);
//	auto it = m_texturesMap.find(autoId);
//	if (it != m_texturesMap.end())
//	{
//		SDL_Log("Lib/AssetManager.cpp/AddTextureByPath/GetNameFromPath/Texture already loaded with ID : %s", autoId.c_str());
//		return;
//	}
//	SDL_Texture* texture = IMG_LoadTexture(m_renderer, p_path.c_str());
//	if (texture == nullptr)
//	{
//		SDL_Log("Lib/AssetManager.cpp/AddTextureByPath/IMG_LoadTexture/failed to load texture: %s", SDL_GetError());
//		return;
//	}
//	m_texturesMap[autoId] = m_texturesVector.size();
//	m_texturesVector.push_back(texture);
//	m_textureIdsVector.push_back(autoId);
//}