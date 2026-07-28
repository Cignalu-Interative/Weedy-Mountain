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
	Debug* dg = Debug::Get();

	m_zone = GetScene()->CreateEntity<Entity>(p_position, p_dimension, "", Shape::Rectangle, true);
	m_zone->GetCollider()->SetIsTrigger(true);
	m_zone->SetDrawLayer(100);
	m_zone->ShowCollider(true);
	m_zone->GetSprite()->SetDimension(p_dimension);

	m_icone = GetScene()->CreateEntity<Entity>(p_position, m_dimSpriteRequirement, "", Shape::Rectangle, true);
	m_icone->SetDrawLayer(150);
}

Quest::~Quest()
{


}

void Quest::AddRequirement(CheckQuest p_CheckType, const std::string& p_idTexture, int p_requiredAmount)
{
	QuestRequirement* requirement = new QuestRequirement();
	requirement->s_requiredAmount = p_requiredAmount;
	requirement->SetCheckType(p_CheckType);
	std::string NameItem = p_idTexture;
	std::transform(NameItem.begin(), NameItem.end(), NameItem.begin(),
		[](unsigned char c) { return std::toupper(c); }
	);
	requirement->SetItemData(ItemDataBase::Get()->GetItem(NameItem));
	m_icone->AddSprite("requirementSprite_" + std::to_string(m_requirements.size()), p_idTexture, m_dimSpriteRequirement, 0, { 0,0 }, true);
	requirement->s_harvestedText = Debug::Get()->CreateText("requirementTextHarvested_" + std::to_string(m_requirements.size()), { 0,0 }, std::to_string(requirement->s_harvestedAmount), ColorsNamed::Green, true, true);
	requirement->s_requiredText = Debug::Get()->CreateText("requirementText_" + std::to_string(m_requirements.size()), { 0,0 }, std::to_string(requirement->s_requiredAmount), ColorsNamed::Red, true, true);
	requirement->s_requiredText->SetDrawLayer(200);
	requirement->s_harvestedText->SetDrawLayer(200);
	m_requirements[m_requirements.size()] = requirement;
	ResizeGrid();
}

QuestRequirement* Quest::GetRequirement(size_t p_emplacement) const
{
	auto it = m_requirements.find(p_emplacement);

	if (it != m_requirements.end())
	{
		return it->second;
	}
	else
	{
		std::cerr << "Quest::GetRequirement/Requirement at emplacement " << p_emplacement << " not found." << std::endl;
		return nullptr;
	}
}

void Quest::AddReward(ItemData* p_itemData, int p_amount)
{
	QuestReward* reward = new QuestReward();
	reward->SetItemData(p_itemData);
	reward->SetAmount(p_amount);
	m_rewards[m_rewards.size()] = reward;
}

QuestReward* Quest::GetReward(size_t p_emplacement) const
{
	auto it = m_rewards.find(p_emplacement);

	if (it != m_rewards.end())
	{
		return it->second;
	}
	else
	{
		std::cerr << "Quest::GetReward/Reward at emplacement " << p_emplacement << " not found." << std::endl;
		return nullptr;
	}
}

void Quest::UpdateQuest()
{
	SetTextQuest();
	ResizeGrid();

	for (auto& [index, requirement] : m_requirements)
	{
		if (requirement->s_isCompleted == false)
		{
			break;
		}
		m_QuestIsCompleted = true;
	}
}

void Quest::SetTextQuest()
{
	for (auto& [index, requirement] : m_requirements)
	{
		requirement->s_harvestedText->SetText(std::to_string(requirement->s_harvestedAmount));
		requirement->s_requiredText->SetText(std::to_string(requirement->s_requiredAmount));
	}
	for (int i = 0; i < m_requirements.size(); ++i)
	{
		//std::cerr << "item : " << m_requirements[i]->GetItemData()->GetName() << " Harvested: " << m_requirements[i]->s_harvestedAmount << " / Required: " << m_requirements[i]->s_requiredAmount << std::endl;
	}
}

void Quest::ResizeGrid()
{
	Vector2i maxSize = m_zone->GetDimension();
	Vector2f posGrid = m_zone->GetPosition();
	int NBElement = (int)m_requirements.size();
	if (NBElement == 0)
		return;

	Vector2i dimSprite = m_dimSpriteRequirement;

	Vector2f dimentionFinalIcone;

	int maxColumns = maxSize.GetX() / dimSprite.GetX(); // soit exemple 128 / 32 = 4 colonnes max

	int nbRows = std::ceil((float)NBElement / maxColumns); // nombre de lignes nécessaires pour afficher tous les éléments ceil pour arrondire au supérieur

	int columnsUsed = std::min(NBElement, maxColumns); // nombre de colonnes utilisées pour afficher tous les éléments
	float widthUsed = columnsUsed * dimSprite.GetX(); // largeur totale utilisée pour afficher tous les éléments
	float heightUsed = nbRows * dimSprite.GetY(); // hauteur totale utilisée pour afficher tous les éléments

	m_zone->Transformable::SetDimension({ (int)widthUsed, (int)heightUsed });
	m_icone->Transformable::SetDimension({ (int)widthUsed, (int)heightUsed });

	float startX = -(columnsUsed - 1) * 0.5f * dimSprite.GetX();
	float startY = -(nbRows - 1) * 0.5f * dimSprite.GetY();

	int spacing = 32; // Espace entre les icônes

	startX -= spacing;

	for (int i = 0; i < NBElement; ++i)
	{
		int row = i / maxColumns;
		int col = i % maxColumns;

		float offsetX = startX + col * (dimSprite.GetX() + spacing);
		float offsetY = startY + row * dimSprite.GetY();

		m_icone->GetSpriteContainer("requirementSprite_" + std::to_string(i))->SetOffset({ offsetX, offsetY });
	}
	m_icone->SetPosition(m_zone->GetPosition());

	for (auto& [index, requirement] : m_requirements)
	{
		Sprite* sprite = m_icone->GetSprite("requirementSprite_" + std::to_string(index));
		Vector2f spritePosLeftDown = sprite->GetPosition({ 0.f, 0.5f });
		Vector2f spritePosRightDown = sprite->GetPosition({ 1.f, 1.f });
		requirement->s_harvestedText->SetPosition({ spritePosLeftDown.GetX(), spritePosLeftDown.GetY() + 10 });
		requirement->s_requiredText->SetPosition({ spritePosRightDown.GetX(), spritePosRightDown.GetY() - 10 });
	}
}

void Quest::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{

}
