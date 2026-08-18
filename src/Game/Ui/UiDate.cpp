#include "pch.h"
#include "UiDate.h"
#include "GameData.h"
#include "2DPendentLib/Debug.h"
#include "SpriteContainer.h"

void UiDate::OnInitialize()
{
	AddSprite("Calendar", "Calendar", { 32,32 }, 0, {}, true);

	Text* t = Debug::Get()->CreateText("Date",GetPosition(), std::to_string(m_gameData.GetDay()) + "/" + std::to_string(m_gameData.GetMonth()) + "/" + std::to_string(m_gameData.GetYear()), {255, 255, 255, 255}, true);

	Vector2f textLeftPosition = Debug::Get()->GetText("Date")->GetPosition({0,0.5f}) - Vector2f(15, 0);

	GetSprite("Calendar")->SetPosition(textLeftPosition, { 1,0.5f });

	GetCollider()->SetIsTrigger(true);

	SetDrawLayer(INT_MAX - 10);
	t->SetDrawLayer(INT_MAX - 10);
}

void UiDate::OnUpdate()
{
	Debug::Get()->GetText("Date")->SetText(std::to_string(m_gameData.GetDay()) + "/" + std::to_string(m_gameData.GetMonth()) + "/" + std::to_string(m_gameData.GetYear()));
}
