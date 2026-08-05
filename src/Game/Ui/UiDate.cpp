#include "pch.h"
#include "UiDate.h"
#include "GameData.h"
#include "2DPendentLib/Debug.h"
#include "SpriteContainer.h"

void UiDate::OnInitialize()
{
	RemoveCollider("default");

	AddSprite("Calendar", "Calendar", { 32,32 }, 0, {}, true);

	Debug::Get()->CreateText("Date",GetPosition(), std::to_string(m_gameData.GetDay()) + "/" + std::to_string(m_gameData.GetMonth()) + "/" + std::to_string(m_gameData.GetYear()), {255, 255, 255, 255}, true);

	Vector2f textLeftPosition = Debug::Get()->GetText("Date")->GetPosition({0,0.5f}) - Vector2f(15, 0);

	GetSprite("Calendar")->SetPosition(textLeftPosition, { 1,0.5f });
}

void UiDate::OnUpdate()
{
	Debug::Get()->GetText("Date")->SetText(std::to_string(m_gameData.GetDay()) + "/" + std::to_string(m_gameData.GetMonth()) + "/" + std::to_string(m_gameData.GetYear()));
}

void UiDate::SetPosition(Vector2f p_position, Vector2f p_anchor )
{
	Entity::SetPosition(p_position, p_anchor);

	Debug::Get()->GetText("Date")->SetPosition(p_position);
	Vector2f textLeftPosition = Debug::Get()->GetText("Date")->GetPosition({ 0,0.5f }) - Vector2f(15, -20);

	GetSprite("Calendar")->SetPosition(textLeftPosition, { 1,0.5f });
}
