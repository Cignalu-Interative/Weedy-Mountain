#include "pch.h"
#include "Quest.h"
#include "2DPendentLib/Debug.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentEngine/SpriteContainer.h"

Quest::Quest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner, Scene* p_scene)
{
	SetName(p_name);
	SetQuestOwner(p_owner);
	SetScene(p_scene);

	m_zone = GetScene()->CreateEntity<Entity>(p_position, p_dimension, "", Shape::Rectangle, true);
	m_zone->GetCollider()->SetIsTrigger(true);
	m_zone->SetDrawLayer(100);
	m_zone->ShowCollider(true);
	m_zone->GetSprite()->SetDimension(p_dimension);

	m_icone = GetScene()->CreateEntity<Entity>(p_position, m_dimSpriteObjective, "", Shape::Rectangle, true);
	m_icone->SetDrawLayer(150);
}

Quest::~Quest()
{
	for (QuestObjective* objective : m_objectives)
		delete objective;

	for (QuestReward* reward : m_rewards)
		delete reward;
}

void Quest::AddObjective(QuestObjective* p_objective)
{
	if (p_objective == nullptr)
		return;

	size_t index = m_objectives.size();

	m_icone->AddSprite("objectiveSprite_" + std::to_string(index), p_objective->GetIconTextureID(), m_dimSpriteObjective, 0, { 0,0 }, true);

	Text* progressText = Debug::Get()->CreateText("questProgressText_" + m_name + "_" + std::to_string(index), { 0,0 }, std::to_string(p_objective->GetCurrentAmount()), ColorsNamed::Green, true, true);
	Text* requiredText = Debug::Get()->CreateText("questRequiredText_" + m_name + "_" + std::to_string(index), { 0,0 }, std::to_string(p_objective->GetRequiredAmount()), ColorsNamed::Red, true, true);

	progressText->SetDrawLayer(200);
	requiredText->SetDrawLayer(200);

	m_objectives.push_back(p_objective);
	m_progressTexts.push_back(progressText);
	m_requiredTexts.push_back(requiredText);

	ResizeGrid();
	SetVisible(GetVisible());
}

QuestObjective* Quest::GetObjective(size_t p_index) const
{
	if (p_index >= m_objectives.size())
	{
		std::cerr << "Quest::GetObjective/index " << p_index << " out of range for quest '" << m_name << "'" << std::endl;
		return nullptr;
	}

	return m_objectives[p_index];
}

void Quest::AddReward(ItemData* p_itemData, int p_amount)
{
	QuestReward* reward = new QuestReward();
	reward->SetItemData(p_itemData);
	reward->SetAmount(p_amount);
	m_rewards.push_back(reward);
}

QuestReward* Quest::GetReward(size_t p_index) const
{
	if (p_index >= m_rewards.size())
	{
		std::cerr << "Quest::GetReward/index " << p_index << " out of range for quest '" << m_name << "'" << std::endl;
		return nullptr;
	}

	return m_rewards[p_index];
}

void Quest::OnEvent(const QuestEvent& p_event)
{
	if (m_state != QuestState::InProgress)
		return;

	for (QuestObjective* objective : m_objectives)
		objective->OnEvent(p_event);

	RefreshVisuals();
}

bool Quest::AreAllObjectivesCompleted() const
{
	if (m_objectives.empty())
		return false;

	for (QuestObjective* objective : m_objectives)
	{
		if (objective->IsCompleted() == false)
			return false;
	}

	return true;
}

void Quest::RefreshVisuals()
{
	for (size_t i = 0; i < m_objectives.size(); ++i)
	{
		m_progressTexts[i]->SetText(std::to_string(m_objectives[i]->GetCurrentAmount()));
		m_requiredTexts[i]->SetText(std::to_string(m_objectives[i]->GetRequiredAmount()));
	}
}

void Quest::ResizeGrid()
{
	Vector2i maxSize = m_zone->GetDimension();
	int nbElement = (int)m_objectives.size();
	if (nbElement == 0)
		return;

	Vector2i dimSprite = m_dimSpriteObjective;

	int maxColumns = maxSize.GetX() / dimSprite.GetX();
	if (maxColumns <= 0)
		maxColumns = 1;

	int nbRows = std::ceil((float)nbElement / maxColumns);
	int columnsUsed = std::min(nbElement, maxColumns);
	float widthUsed = columnsUsed * dimSprite.GetX();
	float heightUsed = nbRows * dimSprite.GetY();

	Vector2f posWantedForZone = m_zone->GetPosition();
	m_zone->Transformable::SetDimension({ (int)widthUsed, (int)heightUsed });
	m_icone->Transformable::SetDimension({ (int)widthUsed, (int)heightUsed });
	m_zone->SetPosition(posWantedForZone);

	int spacing = 32;
	float stepX = dimSprite.GetX() + spacing;
	float startX = -(columnsUsed - 1) * 0.5f * stepX;
	float startY = -(nbRows - 1) * 0.5f * dimSprite.GetY();

	for (int i = 0; i < nbElement; ++i)
	{
		int row = i / maxColumns;
		int col = i % maxColumns;

		float offsetX = startX + col * (dimSprite.GetX() + spacing);
		float offsetY = startY + row * dimSprite.GetY();

		m_icone->GetSpriteContainer("objectiveSprite_" + std::to_string(i))->SetOffset({ offsetX, offsetY });
	}
	m_icone->SetPosition(m_zone->GetPosition());

	for (size_t i = 0; i < m_objectives.size(); ++i)
	{
		Sprite* sprite = m_icone->GetSprite("objectiveSprite_" + std::to_string(i));
		Vector2f spritePosLeftDown = sprite->GetPosition({ 0.f, 0.5f });
		Vector2f spritePosRightDown = sprite->GetPosition({ 1.f, 1.f });
		m_progressTexts[i]->SetPosition({ spritePosLeftDown.GetX(), spritePosLeftDown.GetY() + 10 });
		m_requiredTexts[i]->SetPosition({ spritePosRightDown.GetX(), spritePosRightDown.GetY() - 10 });
	}
}

void Quest::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{

}

std::string Quest::GetRewardDescription() const
{
	std::string rewardDescription;

	for (QuestReward* reward : m_rewards)
	{
		if (reward == nullptr || reward->GetItemData() == nullptr)
			continue;

		rewardDescription += reward->GetItemData()->GetName() + " x" + std::to_string(reward->GetAmount()) + "       ";
	}

	return rewardDescription;
}

std::string Quest::GetObjectivesDescription() const
{
	std::string objectivesDescription;

	for (QuestObjective* objective : m_objectives)
	{
		objectivesDescription += objective->GetDescription() + "             ";
	}

	return objectivesDescription;
}

void Quest::SetVisible(bool p_visible)
{
	m_zone->SetIsVisible(p_visible);
	m_icone->SetIsVisible(p_visible);

	for (size_t i = 0; i < m_objectives.size(); ++i)
	{
		m_progressTexts[i]->SetIsVisible(p_visible);
		m_requiredTexts[i]->SetIsVisible(p_visible);
	}

	m_zone->GetCollider()->SetShow(p_visible);
}

void Quest::Save(nlohmann::json& p_out) const
{
	p_out["state"] = (int)m_state;
	p_out["rewardClaimed"] = m_rewardClaimed;

	nlohmann::json objectivesJson = nlohmann::json::array();

	for (QuestObjective* objective : m_objectives)
	{
		nlohmann::json objJson;
		objective->Save(objJson);
		objectivesJson.push_back(objJson);
	}

	p_out["objectives"] = objectivesJson;
}

void Quest::Load(const nlohmann::json& p_in)
{
	m_state = (QuestState)p_in.value("state", (int)QuestState::NotStarted);
	m_rewardClaimed = p_in.value("rewardClaimed", false);

	if (p_in.contains("objectives") && p_in["objectives"].is_array())
	{
		const auto& arr = p_in["objectives"];

		for (size_t i = 0; i < m_objectives.size() && i < arr.size(); ++i)
			m_objectives[i]->Load(arr[i]);
	}

	RefreshVisuals();
	SetVisible(m_state == QuestState::InProgress);
}