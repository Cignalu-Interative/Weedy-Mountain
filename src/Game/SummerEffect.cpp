#include "pch.h"
#include "SummerEffect.h"
#include "SpriteContainer.h"
#include "GameData.h"

void SummerEffect::OnInitialize()
{
	AddSprite("SummerEffect", "SummerEffect", GetScene()->GetWindowSize(), 0, { 0.f, 0.f }, true);
	GetSprite("SummerEffect")->SetPosition(GetPosition());
	GetSprite("SummerEffect")->SetOpacity(0);

	SetDrawLayer(5002);
}

void SummerEffect::OnUpdate()
{
	if (GameData::Get().GetSeason() == Season::Summer && GetSprite("SummerEffect")->GetOpacity() != 70)
	{
		GetSprite("SummerEffect")->SetOpacity(70);
	}
	else if (GameData::Get().GetSeason() != Season::Summer && GetSprite("SummerEffect")->GetOpacity() != 0)
	{
		GetSprite("SummerEffect")->SetOpacity(0);
	}
}
