#pragma once
#include "Scene.h"

class EscapeMenu : public Scene
{
private :
	
	Scene* m_sceneToReturn = nullptr;

	Button* m_continueButton = nullptr;
	Button* m_settingsButton = nullptr;
	Button* m_quitButton = nullptr;
	Button* m_saveButton = nullptr;


	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;

	void OnDraw() override {};

public :

	void SetSceneToReturn(Scene* p_scene) { m_sceneToReturn = p_scene; }
};

