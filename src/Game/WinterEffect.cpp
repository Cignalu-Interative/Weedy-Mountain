#include "pch.h"
#include "WinterEffect.h"
#include "SpriteContainer.h"
#include "GameData.h"

void WinterEffect::OnInitialize()
{
	AddSprite("WinterEffect", "WinterEffect", GetScene()->GetWindowSize(), 0, { 0.f, 0.f }, true);
	GetSprite("WinterEffect")->SetPosition(GetPosition());
	GetSprite("WinterEffect")->SetOpacity(0);
}

void WinterEffect::OnUpdate()
{
	if (GameData::Get().GetSeason() == Season::Winter && GetSprite("WinterEffect")->GetOpacity() != 255)
	{
		GetSprite("WinterEffect")->SetOpacity(255);
	}
	else if (GameData::Get().GetSeason() != Season::Winter && GetSprite("WinterEffect")->GetOpacity() != 0)
	{
		GetSprite("WinterEffect")->SetOpacity(0);
	}
}
