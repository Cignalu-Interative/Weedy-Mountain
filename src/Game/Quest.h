#pragma once
#include <string>
#include <vector>
#include "2DPendentEngine/StateMachine.h"
#include "2DPendentEngine/Entity.h"


struct QuestRequirement
{
	std::string m_itemID;
	int m_requiredAmount = 0;
	int m_harvestedAmount = 0;

	std::string GetItemID() const { return m_itemID; }

	void SetRequiredAmount(int p_amount) { m_requiredAmount = p_amount; }
	void AddHarvestedAmount(int p_amount) { m_harvestedAmount += p_amount; }

	int GetRequiredAmount() const { return m_requiredAmount; }
	int GetHarvestedAmount() const { return m_harvestedAmount; }
};

struct QuestReward
{
	std::string m_itemID; // id pas trouvé dans la base de donnée alors thune
	int m_amount = 0;

	std::string GetItemID() const { return m_itemID; }

	void SetAmount(int p_amount) { m_amount = p_amount; }

	int GetAmount() const { return m_amount; }
};

class Quest
{
private:

	std::string m_name;
	Entity* m_questOwner = nullptr;

	bool m_isNotStarted = true;
	bool m_isInProcess = false;
	bool m_isFinished = false;

	// signifie que le joueur perdra l'objet que la quête demande à la fin de la quête
	bool m_lostItem = false;

	std::vector<QuestRequirement> m_requirements;

public:

	Quest(const std::string& p_name, Entity* p_owner);
	~Quest();

	void SetName(const std::string& p_name) { m_name = p_name; }
	const std::string& GetName() const { return m_name; }

	void SetQuestOwner(Entity* p_owner) { m_questOwner = p_owner; }
	Entity* GetQuestOwner() const { return m_questOwner; }

	void SetNotStarted() { m_isNotStarted = true; m_isInProcess = false; m_isFinished = false; }
	void SetInProcess() { m_isNotStarted = false; m_isInProcess = true; m_isFinished = false; }
	void SetFinished() { m_isNotStarted = false; m_isInProcess = false; m_isFinished = true; }

	void AddRequirement(const std::string& p_itemID, int p_requiredAmount);
};