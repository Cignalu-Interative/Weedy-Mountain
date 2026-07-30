#pragma once
#include <vector>
#include <string>

struct ItemDisplayInfo
{
    std::string m_name;
    std::string m_description;
    std::string m_iconPath;
    bool        m_unlocked = false;
};

class PlayerCollection
{
private:
    std::vector<std::string> m_unlockedItems;

public:
    void UnlockItem(const std::string& itemID);
    bool IsUnlocked(const std::string& itemID) const;
    const std::vector<std::string>& GetUnlockedItems() const;

    ItemDisplayInfo GetDisplayInfo(const std::string& itemID) const;
};