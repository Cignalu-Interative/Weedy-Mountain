#include "pch.h"
#include "ItemActionRegistry.h"

ItemActionRegistry* ItemActionRegistry::Get()
{
    static ItemActionRegistry instance;
    return &instance;
}

void ItemActionRegistry::Register(const std::string& actionID, ActionFn fn)
{
    if (actionID.empty())
        return;

    m_actions[actionID] = std::move(fn);
}

bool ItemActionRegistry::Execute(const std::string& actionID, Player& player, PlayerInventory& inventory, const std::string& itemID, int slotIndex)
{
    if (actionID.empty())
        return false;

    auto it = m_actions.find(actionID);
    if (it == m_actions.end())
        return false;

    it->second(player, inventory, itemID, slotIndex);
    return true;
}

bool ItemActionRegistry::IsRegistered(const std::string& actionID) const
{
    return m_actions.find(actionID) != m_actions.end();
}