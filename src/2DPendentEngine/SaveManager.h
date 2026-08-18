#pragma once

class Savable;

class SaveManager
{
	std::vector<Savable*> m_providers;
	std::string m_saveFolder = "../../save/";

	std::string m_activeSaveName = "save1";

	SaveManager() = default;

public:
	static SaveManager& Get();

	void SetActiveSaveName(const std::string& p_name) { m_activeSaveName = p_name; }
	const std::string& GetActiveSaveName() const { return m_activeSaveName; }

	bool QuickSave() { return SaveToFile(m_activeSaveName); }
	bool SaveExists(const std::string& p_saveName) const;

	void RegisterProvider(Savable* p_provider);
	void UnregisterProvider(Savable* p_provider);

	bool SaveToFile(const std::string& p_saveName);
	bool LoadFromFile(const std::string& p_saveName);

	bool PrepareSceneForLoad(const std::string& p_saveName);
};

