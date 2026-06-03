#include "pch.h"
#include "main.h"
#include <SDL.h>
#include "2DPendentEngine/Application.h"
#include "2DPendentEngine/Aleatory.h"
#include "Launcher.h"

//Code d'entrée
int main(int argc, char* argv[])
{
	Aleatory::Initialize();
	Application* application = Application::Get();
	application->Init();

	Launcher::Launch();

	application->Run();
	application->Shutdown();

	return 0;
}