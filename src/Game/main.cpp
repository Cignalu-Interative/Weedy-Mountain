#include "pch.h"
#include "main.h"
#include <SDL.h>
#include "2DPendentEngine/Application.h"
#include "2DPendentEngine/Aleatory.h"
#include "DemoScene.h"
#include "TestScene.h"

//Code d'entrée
int main(int argc, char* argv[])
{
	Aleatory::Initialize();
	Application* application = Application::Get();
	application->Init();

	SceneManager::Get()->AddScene<TestScene>("demo");

	application->Run();
	application->Shutdown();

	return 0;
}