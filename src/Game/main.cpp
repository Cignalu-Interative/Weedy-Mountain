#include "pch.h"
#include "main.h"
#include <SDL3/SDL.h>
#include "2DPendentEngine/Application.h"
#include "2DPendentEngine/Aleatory.h"
#include "Launcher.h"

//Code d'entr�e
int main(int argc, char* argv[])
{
	Aleatory::Initialize();
	Application* application = Application::Get();
	application->Init();
	//application->GetWindow()->SetWindowSize({ 1920, 1080 });

	Launcher::Launch();

	application->Run();
	application->Shutdown();

	return 0;
}