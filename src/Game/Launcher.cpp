#include "pch.h"
#include "Launcher.h"
#include "SceneManager.h"
#include "DemoScene.h"

namespace Launcher
{
	void Launch()
	{
		SceneManager::Get()->AddScene<DemoScene>("demo");
	}
}