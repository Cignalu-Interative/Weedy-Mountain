#include "pch.h"
#include "Launcher.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "TestScene.h"
#include "GameScene.h"
#include "CaveScene.h"

namespace Launcher
{
	void Launch()
	{
		SceneManager::Get()->AddScene<GameScene>("game");
		SceneManager::Get()->AddScene<CaveScene>("cave");
		SceneManager::Get()->GetScene("cave")->SetIsVisible(false);
		//SceneManager::Get()->AddScene<DemoScene>("demo");
		//SceneManager::Get()->AddScene<TestScene>("test");
	}
}