#include "pch.h"
#include "Launcher.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "DemoScene.h"
#include "TestScene.h"

namespace Launcher
{
	void Launch()
	{
		SceneManager::Get()->AddScene<DemoScene>("demo");
		//SceneManager::Get()->AddScene<TestScene>("test");
		//SceneManager::Get()->AddScene<GameScene>("game");
	}
}