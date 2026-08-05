#include "pch.h"
#include "UiTime.h"
#include "SpriteContainer.h"

void UiTime::OnInitialize()
{
	RemoveCollider("default");

	AddSprite("Clock", "Clock", { 32,32 }, 0, {}, true);

	Debug::Get()->CreateText("time", GetPosition(), std::to_string(m_gameData.GetHour()) + ":" + std::to_string(m_gameData.GetMinute()), { 255, 255, 255, 255 }, true);

	Vector2f textLeftPosition = Debug::Get()->GetText("time")->GetPosition({ 0,0.5f }) - Vector2f{20.f, 0.f};

	GetSprite("Clock")->SetPosition(textLeftPosition, { 1,0.5f });

}

void UiTime::OnUpdate()
{
	Debug::Get()->GetText("time")->SetText(std::to_string(m_gameData.GetHour()) + ":" + std::to_string(m_gameData.GetMinute()));
}

void UiTime::SetPosition(Vector2f p_position, Vector2f p_anchor)
{
	Entity::SetPosition(p_position, p_anchor);
	Vector2f textLeftPosition = Debug::Get()->GetText("time")->GetPosition({ 0,0.5f }) - Vector2f{ 20.f, 0.f };

	GetSprite("Clock")->SetPosition(GetPosition() + Vector2f{0, 20});
	Debug::Get()->GetText("time")->SetPosition(GetSprite("Clock")->GetPosition({1,0.5}) + Vector2f{25,0});
}
