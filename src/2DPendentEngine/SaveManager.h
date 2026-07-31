#pragma once

class Savable;

class SaveManager
{
	std::vector<Savable*> m_providers;
	std::string m_saveFolder = "../../save/";

	SaveManager();

public:
	static SaveManager& Get();

	void RegisterProvider(Savable* p_provider);
	void UnregisterProvider(Savable* p_provider);

	bool SaveToFile(const std::string& p_saveName);
	bool LoadFromFile(const std::string& p_saveName);
};

