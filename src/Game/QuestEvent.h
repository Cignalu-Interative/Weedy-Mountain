#pragma once
#include <string>

enum class QuestEventType
{
	UNDEFINED,

	ItemCollected,      
	TerrainCleared,    
	HoleDug,         
	SeedPlanted,        
	PlantWatered,      
	PlantHarvested,     
	ItemSold,         
	ItemCrafted,      
	RewardClaimed,      

	Max
};

struct QuestEvent
{
	QuestEventType m_type = QuestEventType::UNDEFINED;
	std::string m_itemID;  
	int m_amount = 1;     

	QuestEvent() = default;
	QuestEvent(QuestEventType p_type, const std::string& p_itemID = "", int p_amount = 1) : m_type(p_type), m_itemID(p_itemID), m_amount(p_amount) {}
};