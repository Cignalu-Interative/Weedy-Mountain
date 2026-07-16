#pragma once

#include <string>
#include <unordered_map>
#include <functional>

class PlayerInventory;

class ItemActionRegistry
{
public:

    using ActionFn = std::function<void(PlayerInventory& inventory, const std::string& itemID, int slotIndex)>;

    static ItemActionRegistry* Get();

    void Register(const std::string& actionID, ActionFn fn);

    bool Execute(const std::string& actionID, PlayerInventory& inventory, const std::string& itemID, int slotIndex);

    bool IsRegistered(const std::string& actionID) const;

private:

    ItemActionRegistry() = default;

    std::unordered_map<std::string, ActionFn> m_actions;
};