#pragma once
#include <vector>
#include <string>

#include "Savable.h"

struct ItemDisplayInfo
{
    std::string m_name;
    std::string m_description;
    std::string m_iconPath;
    std::string m_texturePath;
    bool m_unlocked = false;
};

class PlayerCollection : public Savable
{
private:
    std::vector<std::string> m_unlockedItems;

public:
	PlayerCollection();
    ~PlayerCollection();

    void UnlockItem(const std::string& itemID);
    bool IsUnlocked(const std::string& itemID) const;
    const std::vector<std::string>& GetUnlockedItems() const;

    ItemDisplayInfo GetDisplayInfo(const std::string& itemID) const;

    std::string GetSaveID() const override { return "PlayerCollection"; }
    void Save(json& p_out) override;
	void Load(const json& p_in) override;
};