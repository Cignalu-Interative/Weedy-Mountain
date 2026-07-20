#include "pch.h"
#include "Quest.h"

Quest::Quest(const std::string& p_name, Entity* p_owner)
{
	SetName(p_name);
	SetQuestOwner(p_owner);
}

Quest::~Quest()
{


}

void Quest::AddRequirement(const std::string& p_itemID, int p_requiredAmount)
{
	QuestRequirement requirement;
	requirement.m_itemID = p_itemID;
	requirement.m_requiredAmount = p_requiredAmount;
	m_requirements.push_back(requirement);
}
