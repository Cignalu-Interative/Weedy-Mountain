#include "pch.h"
#include "UiTime.h"
#include "SpriteContainer.h"

void UiTime::OnInitialize()
{
	AddSprite("Clock", "Clock", { 32,32 }, 0, {}, true);

	Text* t = Debug::Get()->CreateText("time", GetPosition(), std::to_string(m_gameData.GetHour()) + ":" + std::to_string(m_gameData.GetMinute()), { 255, 255, 255, 255 }, true);

	Vector2f textLeftPosition = Debug::Get()->GetText("time")->GetPosition({ 0,0.5f }) - Vector2f{20.f, 0.f};

	GetSprite("Clock")->SetPosition(textLeftPosition, { 1,0.5f });

	GetCollider()->SetIsTrigger(true);

	SetDrawLayer(INT_MAX - 10);
	t->SetDrawLayer(INT_MAX - 10);
}

void UiTime::OnUpdate()
{
	Debug::Get()->GetText("time")->SetText(std::to_string(m_gameData.GetHour()) + ":" + std::to_string(m_gameData.GetMinute()));
}
