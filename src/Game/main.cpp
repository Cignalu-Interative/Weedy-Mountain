#include "pch.h"
#include "main.h"
#include <SDL.h>
#include "2DPendentEngine/Application.h"
#include "2DPendentEngine/Aleatory.h"
#include "DemoScene.h"
#include "GameScene.h"

//Code d'entr�e
int main(int argc, char* argv[])
{
	Aleatory::Initialize();
	Application* application = Application::Get();
	application->Init();

	SceneManager::Get()->AddScene<DemoScene>("demo");
	SceneManager::Get()->AddScene<GameScene>("game");
	
	SceneManager::Get()->SetCurrentScene("game");

	application->Run();
	application->Shutdown();

	return 0;
}