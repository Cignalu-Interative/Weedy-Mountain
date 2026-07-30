#pragma once
#include "Scene.h"

class Button;
class GlobalMenuScene : public Scene
{

private :

	Button* m_startButton = nullptr;
	Button* m_continueButton = nullptr;
	Button* m_newGameButton = nullptr;
	Button* m_settingsButton = nullptr;
	Button* m_quitButton = nullptr;

	bool m_hasSave = true;

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;

	void OnDraw() override {};

public : 
};

