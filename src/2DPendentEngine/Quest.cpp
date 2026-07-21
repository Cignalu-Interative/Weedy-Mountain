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
	requirement.s_sprite = Debug::Get()->CreateSprite("questRequirement_" + p_idTexture, p_idTexture, m_circleQuest->GetPosition(), {32, 32}, true, true);
	requirement.s_sprite->SetDrawLayer(300);
	requirement.s_requiredAmount = p_requiredAmount;
	m_requirements[m_requirements.size()] = requirement;
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
	ResizeGrid();
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

void Quest::ResizeGrid()
{
	Vector2i dim = m_circleQuest->GetDimension();
	size_t size = m_requirements.size();
	if (size == 0)
		return;

	int cols = std::ceil(std::sqrt(size));
	int rows = std::ceil(size / (float)cols);

	float cellW = dim.GetX() / cols;
	float cellH = dim.GetY() / rows;

	float gridW = cols * cellW;
	float gridH = rows * cellH;

	float startX = (dim.GetX() - gridW) * 0.5f;
	float startY = (dim.GetY() - gridH) * 0.5f;

	for (int i = 0; i < size; ++i)
	{
		int col = i % cols;
		int row = i / cols;

		auto& requirement = m_requirements[i];
		Sprite* sprite = requirement.s_sprite;
		float s = std::min(cellW / sprite->GetDimension().GetX(), cellH / sprite->GetDimension().GetY());
		float drawW = sprite->GetDimension().GetX() * s;
		float drawH = sprite->GetDimension().GetY() * s;

		float x = startX + col * cellW + (cellW - drawW) * 0.5f;
		float y = startY + row * cellH + (cellH - drawH) * 0.5f;

		sprite->SetDimension({ static_cast<int>(drawW), static_cast<int>(drawH) });
		sprite->SetPosition({x, y});
	}

}

void Quest::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	for (auto& [index, requirement] : m_requirements)
	{
		requirement.s_sprite->Draw(f_render, f_window, p_camera);
	}
}
