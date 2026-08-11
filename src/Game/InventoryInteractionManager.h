#pragma once

#include "InventoryInputControler.h"
#include "InventoryStateMachine.h"
#include "InventoryLogicManager.h"
#include "NotificationManager.h"


#include "Button.h"
#include "Entity.h"

class CursorItemUI;

class Inventory;
class Hotbar;
class InputManager;
class PlayerInventory;

class InventoryManager
{

private:

    InventoryLogic m_logic;
    InventoryStateMachine m_stateMachine;
    InventoryInputController m_inputController;

    NotificationManager m_notificationManager;

    PlayerInventory* m_playerInventory = nullptr;

    CursorItemUI* m_cursorUI = nullptr;

    Inventory* m_inventory = nullptr;
    Hotbar* m_hotbar = nullptr;

    Scene* m_scene = nullptr;

    bool m_isInventoryOpen = false;


public:

    InventoryManager() = default;

    void Initialize(Scene* scene, Inventory* inventory, Hotbar* hotbar, PlayerInventory* playerInventory);

    void RegisterInventory(std::vector<InventorySlot*>& slots, uint8_t flags, InventoryLogic::InventoryType type);
    
    InventoryLogic* GetLogic();

    const char* GetState();



    //les trucs du player inventory
    void Open();
    void Close();
    void Toggle();

    bool IsOpen();

    void OnUpdate();
    
private:
    
    void UpdateHotbarSelection(InputManager* input);
    void SelectHoveredHotbarSlot();
    void UpdateHotbarSwap(InputManager* input);

    InventorySlot* GetHoveredSlot();

};






//a bouger mais là j'ai la flemme...


class CursorItemUI : public Button
{
    ItemStack* m_stack = nullptr;

    std::string m_lastID;

public:

    CursorItemUI() = default;


    void OnInitialize() override;
    void OnUpdate() override;


    void SetStack(ItemStack* stack);

private:
    void RefreshVisual();
};