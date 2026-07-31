#pragma once
#include "2DPendentEngine\Scene.h"

class PlayerInventory;
class UiCollection;
class QuestMenu;
class HUDScene : public Scene
{
private:
	PlayerInventory* m_playerInventory = nullptr;
	UiCollection* m_collectionUi = nullptr;
	QuestMenu* m_questMenu = nullptr;

protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override {};

	void OnWake() override {};

	void OnDraw() override {};


public : 
};

