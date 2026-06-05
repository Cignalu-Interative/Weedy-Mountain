#include "pch.h"
#include "TestScene.h"
#include "2DPendentLib/InputManager.h"
#include "Player.h"

inline Debug& debug = *Debug::Get();
inline InputManager& input = *InputManager::Get();
inline SoundManager& sound = *SoundManager::Get();

void TestScene::OnInitialize()
{
	CreateRect("Rect", Vector2f(100, 100), Vector2i(100, 100), Color(255, 0, 0, 255));
	CreateCircle("Circle", Vector2f(200, 200), Vector2i(50, 50), Color(0, 255, 0, 255), false, false);

	CreateEntity<Player>(Vector2f(400, 400), Vector2i(100, 100), "Perceval", Shape::Rectangle);
}

void TestScene::OnEvent()
{
	if (input.IsKeyboardDown(K_TAB))
	{
		//SceneManager::Get()->ClearScenesInStack();
		SceneManager::Get()->PushScene("demo");
	}
}

void TestScene::OnUpdate()
{

}

void TestScene::OnSleep()
{
	SetIsVisible(false);
	SetSleepEvent(true);
	SetSleepUpdate(true);
}

void TestScene::OnWake()
{
	SetIsVisible(true);
	SetSleepEvent(false);
	SetSleepUpdate(false);
}

void TestScene::OnDestroy()
{
	for (Entity* entity : GetEntities())
	{
		entity->Destroy();
	}
	GetEntities().clear();
	debug.ClearAll();
}
