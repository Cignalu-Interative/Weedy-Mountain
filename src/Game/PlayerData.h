#pragma once
#include "Player.h"
#include "Inventory.h"
#include "Hotbar.h"
#include "PlayerInventory.h"

struct PlayerData
{
	PlayerInventory playerInventory;

	PlayerStats playerStats;
	std::vector<PlayerJob*> playerJobs;

	void SavePlayerData(Player* p_player)
	{
		playerInventory = *p_player->GetInventory();
		
		playerJobs = p_player->GetJobs();
		playerStats = p_player->GetStats();

	}

	void PushPlayerData(Player* p_player)
	{
		*p_player->GetInventory() = playerInventory;

		p_player->GetJobs() = playerJobs;
		p_player->GetStats() = playerStats;

	}


};