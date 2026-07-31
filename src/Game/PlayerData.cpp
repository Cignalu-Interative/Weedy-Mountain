#include "pch.h"
#include "PlayerData.h"

void PlayerData::Heal(int p_amount)
{
	m_stats.m_hp += p_amount;
	if (m_stats.m_hp > m_stats.m_maxHp)
		m_stats.m_hp = m_stats.m_maxHp;
}

void PlayerData::TakeDamage(int p_amount)
{
	m_stats.m_hp -= p_amount;
	if (m_stats.m_hp < 0)
		m_stats.m_hp = 0;
}

void PlayerData::AddStamina(int p_amount)
{
	m_stats.m_stamina += p_amount;
	if (m_stats.m_stamina > m_stats.m_maxStamina)
		m_stats.m_stamina = m_stats.m_maxStamina;
}

void PlayerData::DecreaseStamina(int p_amount)
{
	m_stats.m_stamina -= p_amount;
	if (m_stats.m_stamina < 0)
		m_stats.m_stamina = 0;
}

void PlayerData::AddToxicity(int p_amount)
{
	m_stats.m_toxicity += p_amount;
	if (m_stats.m_toxicity > m_stats.m_maxToxicity)
		m_stats.m_toxicity = m_stats.m_maxToxicity;
}

void PlayerData::DecreaseToxicity(int p_amount)
{
	m_stats.m_toxicity -= p_amount;
	if (m_stats.m_toxicity < 0)
		m_stats.m_toxicity = 0;
}

void PlayerData::DecreaseMoney(int p_amount)
{
	m_money -= p_amount;
	if (m_money < 0)
		m_money = 0;
}

void PlayerData::DecreaseCash(int p_amount)
{
	m_cash -= p_amount;
	if (m_cash < 0)
		m_cash = 0;
}

void PlayerData::AddXpToJob(JobType p_type, float p_amount)
{
	PlayerJob* job = GetJobByName(p_type);

	if (job != nullptr)
		job->m_exp += p_amount;
}

PlayerJob* PlayerData::GetJobByName(JobType p_type)
{
	for (PlayerJob* job : m_jobs)
	{
		if (job->m_name == p_type)
			return job;
	}
	return nullptr;
}
