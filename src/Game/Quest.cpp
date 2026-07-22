#include "pch.h"
#include "Quest.h"
#include "2DPendentLib/Debug.h"
#include "2DPendentLib/Utils.h"

Quest::Quest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner)
{
	SetName(p_name);
	SetQuestOwner(p_owner);
	Debug* dg = Debug::Get();

	m_circleQuest = dg->CreateFillCircle("questCircle_" + p_name, p_position, p_dimension, { 255, 255, 255, 255 }, true, true);
	m_circleQuest->SetDrawLayer(100);

	m_textQuest = dg->CreateText("questText_" + p_name, p_position, "", ColorsNamed::Green, true, true);
	m_textQuest->SetFontSize(18);
	m_textQuest->SetDrawLayer(200);
}

Quest::~Quest()
{


}

void Quest::AddRequirement(const std::string& p_idTexture, int p_requiredAmount)
{
	QuestRequirement requirement;
	requirement.s_icone = GetScene()->CreateEntity<Entity>(m_circleQuest->GetPosition(), m_dimSpriteRequirement, p_idTexture, Shape::Rectangle, true);
	requirement.s_icone->SetDrawLayer(150);
	requirement.s_requiredAmount = p_requiredAmount;
	m_requirements[m_requirements.size()] = requirement;
	//ResizeGrid();
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
	for (auto& [index, requirement] : m_requirements)
	{
		requirement.s_icone->SetPosition(m_circleQuest->GetPosition());
	}
}

void Quest::ResizeGrid()
{
	Vector2i maxSize = m_circleQuest->GetDimension();
	Vector2f posGrid = m_circleQuest->GetPosition();
	int NBElement = (int)m_requirements.size();
	if (NBElement == 0)
		return;

	Vector2i dimSprite = m_dimSpriteRequirement;

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
