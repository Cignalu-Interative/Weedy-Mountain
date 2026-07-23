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

	m_textQuest = dg->CreateText("questText_" + p_name, p_position, "", ColorsNamed::Green, true, true);
	m_textQuest->SetFontSize(18);
	m_textQuest->SetDrawLayer(200);

	m_icone = GetScene()->CreateEntity<Entity>(p_position, m_dimSpriteRequirement, "", Shape::Rectangle, true);
	m_icone->SetDrawLayer(150);
}

Quest::~Quest()
{


}

void Quest::AddRequirement(CheckQuest p_CheckType, const std::string& p_idTexture, int p_requiredAmount)
{
	QuestRequirement requirement;
	requirement.s_requiredAmount = p_requiredAmount;
	requirement.SetCheckType(p_CheckType);
	std::string NameItem = p_idTexture;
	std::transform(NameItem.begin(), NameItem.end(), NameItem.begin(),
		[](unsigned char c) { return std::toupper(c); }
	);
	requirement.SetItemData(ItemDataBase::Get()->GetItem(NameItem));
	if (requirement.GetItemData() == nullptr)
	{
		std::cerr << "Quest::AddRequirement/ItemData for item '" << NameItem << "' not found." << std::endl;
		return;
	}
	m_icone->AddSprite("requirementSprite_" + std::to_string(m_requirements.size()), p_idTexture, m_dimSpriteRequirement, 0, { 0,0 }, true);
	requirement.s_harvestedText = Debug::Get()->CreateText("requirementTextHarvested_" + std::to_string(m_requirements.size()), { 0,0 }, std::to_string(requirement.s_harvestedAmount), ColorsNamed::Green, true, true);
	requirement.s_requiredText = Debug::Get()->CreateText("requirementText_" + std::to_string(m_requirements.size()), { 0,0 }, std::to_string(requirement.s_requiredAmount), ColorsNamed::Red, true, true);
	m_requirements[m_requirements.size()] = requirement;
	ResizeGrid();
}

QuestRequirement Quest::GetRequirement(size_t p_emplacement) const
{
	auto it = m_requirements.find(p_emplacement);

	if (it != m_requirements.end())
	{
		return it->second;
	}
	else
	{
		std::cerr << "Quest::GetRequirement/Requirement at emplacement " << p_emplacement << " not found." << std::endl;
		return QuestRequirement();
	}
}

void Quest::UpdateQuest()
{
	SetTextQuest();
	SetSpriteQuest();
	ResizeGrid();

	//std::cerr << "circle center pos = (" << m_circleQuest->GetPosition().GetX() << ", " << m_circleQuest->GetPosition().GetY() << ")" << std::endl;
	//std::cerr << "sprite pos = (" << m_requirements[0].s_sprite->GetPosition().GetX() << ", " << m_requirements[0].s_sprite->GetPosition().GetY() << ")" << std::endl;
}

void Quest::SetTextQuest()
{
	//m_textQuest->SetText(" ");
	//for (auto& [index, requirement] : m_requirements)
	//{
	//	//m_textQuest->SetText(m_textQuest->GetText() + "Item ID: " + requirement.GetItemID() + ", Required: " + std::to_string(requirement.GetRequiredAmount()) + ", Harvested: " + std::to_string(requirement.GetHarvestedAmount()) + "\n");
	//}
	//m_textQuest->SetPosition(m_circleQuest->GetPosition());
}

void Quest::SetSpriteQuest()
{
	/*for (auto& [index, requirement] : m_requirements)
	{
		requirement.s_icone->SetPosition(m_circleQuest->GetPosition());
	}*/
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
		requirement.s_harvestedText->SetPosition({ spritePosLeftDown.GetX(), spritePosLeftDown.GetY() + 10 });
		requirement.s_requiredText->SetPosition({ spritePosRightDown.GetX(), spritePosRightDown.GetY() - 10 });
	}
	

	/*Vector2i ecart = {};

	for (int i = 0; i < NBElement; ++i)
	{
		
		m_icone->GetSprite("requirementSprite_" + std::to_string(i))->SetPosition();

	}*/


	/*int floor = 1;

	for (int i = 0; i < NBElement; ++i)
	{
		if ((dimentionFinalIcone.GetX() * floor) + dimSprite.GetX() > maxSize.GetX())
		{
			floor++;
			dimentionFinalIcone.SetY((dimentionFinalIcone.GetY() * floor) + dimSprite.GetY());
		}
		dimentionFinalIcone.SetX((dimentionFinalIcone.GetX() * floor) + dimSprite.GetX());
	}*/

	/*float nexPosX = posGrid.GetX() - (maxSize.GetX() * 0.5f ) / NBElement;

	for (auto& [index, requirement] : m_requirements)
	{
		Sprite* sprite = requirement.s_sprite;
		sprite->SetPosition({ nexPosX, sprite->GetPosition().GetY()});
		nexPosX += maxSize.GetX() / NBElement;
	}*/

	/*Vector2i dim = m_circleQuest->GetDimension();
	Vector2f pos = m_circleQuest->GetPosition();
	size_t size = m_requirements.size();
	if (size == 0)
		return;

	int cols = std::ceil(std::sqrtf(size));
	int rows = std::ceil(size / (float)cols);

	float spacing = 8.0f;

	float cellW = (dim.GetX() - spacing * (cols - 1)) / cols;
	float cellH = (dim.GetY() - spacing * (rows - 1)) / rows;

	float gridW = cols * cellW + (cols - 1) * spacing;
	float gridH = rows * cellH + (rows - 1) * spacing;

	float startX = pos.GetX() + (dim.GetX() - gridW) * 0.5f;
	float startY = pos.GetY() + (dim.GetY() - gridH) * 0.5f;

	for (int i = 0; i < size; ++i)
	{
		int col = i % cols;
		int row = i / cols;

		auto& requirement = m_requirements[i];
		Sprite* sprite = requirement.s_sprite;

		float spriteW = (float)sprite->GetDimension().GetX();
		float spriteH = (float)sprite->GetDimension().GetY();

		float s = std::min(cellW / spriteW, cellH / spriteH);

		float drawW = spriteW * s;
		float drawH = spriteH * s;

		float cellX = startX + col * (cellW + spacing);
		float cellY = startY + row * (cellH + spacing);

		float x = cellX + (cellW - drawW) * 0.5f;
		float y = cellY + (cellH - drawH) * 0.5f;

		sprite->SetDimension({ static_cast<int>(drawW), static_cast<int>(drawH) });
		sprite->SetPosition({x, y});
	}*/
}

void Quest::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	/*for (auto& [index, requirement] : m_requirements)
	{
		requirement.s_icone->Draw(f_render, f_window, p_camera);
	}*/
}
