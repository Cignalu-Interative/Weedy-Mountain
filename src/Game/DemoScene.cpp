#include "pch.h"
#include "DemoScene.h"

#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

inline Debug& debug = *Debug::Get();
inline InputManager& input = *InputManager::Get();
inline SoundManager& sound = *SoundManager::Get();

void DemoScene::OnInitialize()
{
	m_gameData.Initialize();

	debug.CreateText("Time", { GetWindowWidth() / 2.0f, 15 }, "00:00", ColorsNamed::White, true);

	Text* timeText = debug.GetText("Time");
	timeText->SetFontSize(75);

	debug.CreateText("Date", { GetWindowWidth() / 2.0f, 100 }, " ", ColorsNamed::White, true);

	Text* dateText = debug.GetText("Date");
	dateText->SetFontSize(35);

	debug.CreateCircle("Clock", { GetWindowWidth() / 2.0f, 175 }, { 100, 100 }, ColorsNamed::Blue, true);

	Circle* circle = debug.GetCircle("Clock");
	circle->SetFilled(true);
}

void DemoScene::OnEvent()
{
	//test pour la perception du temps
	if (input.IsKeyboardDown(K_a))
	{
		m_gameData.SetTimeScale(1.0f);
	}

	if (input.IsKeyboardDown(K_z))
	{
		m_gameData.SetTimeScale(500.0f);
	}

	if (input.IsKeyboardDown(K_s))
	{
		m_gameData.Sleep();
	}
}

void DemoScene::OnUpdate()
{
	m_gameData.Update();

	Text* timeText = debug.GetText("Time");

	if (timeText != nullptr)
	{
		std::string text = "";

		if (m_gameData.GetHour() < 10)
			text += "0";

		text += std::to_string(m_gameData.GetHour());
		text += ":";

		if (m_gameData.GetMinute() < 10)
			text += "0";

		text += std::to_string(m_gameData.GetMinute());

		timeText->SetText(text);
	}

	Text* dateText = debug.GetText("Date");

	if (dateText != nullptr)
	{
		std::string text = "";

		text += m_gameData.GetDayName();
		text += " ";

		text += std::to_string(m_gameData.GetDay());
		text += " ";

		text += m_gameData.GetMonthName();
		text += " ";

		text += std::to_string(m_gameData.GetYear());
		text += " ";

		dateText->SetText(text);
	}

	Circle* circle = debug.GetCircle("Clock");

	if (m_gameData.GetHour() < 6 || m_gameData.GetHour() >= 18)
		circle->SetColor(ColorsNamed::Blue);
	else
		circle->SetColor(ColorsNamed::Yellow);
}

void DemoScene::OnDestroy()
{
	debug.ClearAll();
}