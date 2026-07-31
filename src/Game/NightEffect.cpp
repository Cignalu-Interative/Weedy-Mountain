#include "pch.h"
#include "NightEffect.h"
#include "SpriteContainer.h"
#include "2DPendentLib/Utils.h"

void NightEffect::OnInitialize()
{
	AddSprite("NightEffect", "NightEffect", GetScene()->GetWindowSize(), 0, {0.f, 0.f}, true);
	GetSprite("NightEffect")->SetPosition(GetPosition());
	GetSprite("NightEffect")->SetOpacity(255);

	SetDrawLayer(5000);

	m_season = GameData::Get().GetSeason();

	m_sunrise = 6;
	m_endSunrise = 8;

	m_sunset = 19;
	m_endSunset = 21;
}

void NightEffect::OnUpdate()
{
	int hour = GameData::Get().GetHour();
	int minute = GameData::Get().GetMinute();

	float currentTime = hour + (minute / 60.f);

	Season currentSeason = GameData::Get().GetSeason();
	if (m_season != currentSeason)
	{
		m_season = currentSeason;

		if (m_season == Season::Spring)
		{
			m_sunrise = 6;
			m_endSunrise = 8;

			m_sunset = 19;
			m_endSunset = 21;
		}

		else if (m_season == Season::Summer)
		{
			m_sunrise = 5;
			m_endSunrise = 7;

			m_sunset = 20;
			m_endSunset = 22;
		}

		else if (m_season == Season::Autumn)
		{
			m_sunrise = 6;
			m_endSunrise = 8;

			m_sunset = 19;
			m_endSunset = 21;
		}

		else if (m_season == Season::Winter)
		{
			m_sunrise = 7;
			m_endSunrise = 9;

			m_sunset = 18;
			m_endSunset = 20;
		}
	}

	if (currentTime >= m_sunrise && currentTime <= m_endSunrise)
	{
		float ratio = (currentTime - m_sunrise) / (m_endSunrise - m_sunrise);
		ratio = Utils::Clamp(ratio, 0.f, 1.f);

		int opacity = static_cast<int>(255 * (1.f - ratio));
		GetSprite("NightEffect")->SetOpacity(opacity);
	}


	else if (currentTime >= m_sunset && currentTime <= m_endSunset)
	{
		float ratio = (currentTime - m_sunset) / (m_endSunset - m_sunset);
		ratio = Utils::Clamp(ratio, 0.f, 1.f);

		int opacity = static_cast<int>(255 * ratio);
		GetSprite("NightEffect")->SetOpacity(opacity);
	}

	else if (currentTime > m_endSunrise && currentTime < m_sunset)
	{
		GetSprite("NightEffect")->SetOpacity(0);
	}
	else
	{
		GetSprite("NightEffect")->SetOpacity(255);
	}
}
