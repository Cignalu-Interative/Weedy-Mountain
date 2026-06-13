#include "pch.h"
#include "DemoScene.h"
#include "Player.h"
#include "User.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/Camera.h"
#include "2DPendentEngine/RectangleCollider.h"
#include "2DPendentLib/FontManager.h"

inline Debug& debug = *Debug::Get();
inline InputManager& input = *InputManager::Get();
inline SoundManager& sound = *SoundManager::Get();

DemoScene::DemoScene()
{
	m_players.reserve(64); // Réserver de la mémoire pour 64 entites afin d'éviter les reallocations fréquentes lors de l'ajout de joueurs
}

void DemoScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/2DPendentEngine/Assets/Textures/Textures_Repertory.json");
	MapTiledManager::Get()->AddMaps("../../res/2DPendentEngine/Assets/Maps/Maps_Repertory.json");
	SoundManager::Get()->AddSound("../../res/2DPendentEngine/Assets/Sounds/Sounds_Repertory.json");
	FontManager::Get()->AddFontByRepertory("../../res/2DPendentEngine/Assets/Fonts/Fonts_Repertory.json");


	Player* player1 = CreateEntity<Player>({ 200, 0 }, { 200, 200 }, "Perceval", Shape::Rectangle);
	//player1->GetSprite()->SetOpacity(128);
	player1->SetAngle(0.0f);
	player1->InitialisePhysicsBody(ShapeBodyPhysics::Dynamic);
	player1->GetPhysicsBody()->SetMass(20.f);
	player1->SetRigidBody(true);
	player1->ShowCollider(true);
	m_player1 = player1;
	m_players.push_back(player1);

	Player* player2 = new Player();
	player2 = CreateEntity<Player>({ 200, 400 }, { 500, 100 }, "Perceval", Shape::Rectangle);
	//player2->GetSprite()->SetOpacity(45);
	player2->SetAngle(0.0f);
	player2->InitialisePhysicsBody(ShapeBodyPhysics::Static);
	//player2->GetPhysicsBody()->SetMass(20.f);
	player2->SetRigidBody(true);
	player2->ShowCollider(true);
	m_player2 = player2;
	m_players.push_back(player2);

	//Player* player3 = new Player();
	//player3 = CreateEntity<Player>({ 250, 550 }, { 150, 150 }, "Perceval", Shape::Rectangle);
	//player3->GetSprite()->SetOpacity(67);
	//player3->SetAngle(8.0f);
	//player3->SetRigidBody(true);
	//player3->ShowCollider(true);
	//m_players.push_back(player3);

	//Player* player4 = new Player();
	//player4 = CreateEntity<Player>({ 400, 100 }, { 100, 100 }, "Perceval", Shape::Circle);
	//player4->GetSprite()->SetOpacity(200);
	//player4->SetAngle(0.0f);
	//player4->SetRigidBody(true);
	//player4->ShowCollider(true);
	//m_players.push_back(player4);

	//Player* player5 = new Player();
	//player5 = CreateEntity<Player>({ 1000, 350 }, { 125, 125 }, "Perceval", Shape::Rectangle);
	//player5->SetAngle(0.0f);
	//player5->SetRigidBody(true);
	//player5->ShowCollider(true);
	//m_players.push_back(player5);

	/*for (int i = 0; i < 80; i++)
	{
		Player* player5 = new Player();
		player5 = CreateEntity<Player>({ 200, 325 }, { 200, 200 }, "Perceval", Shape::Rectangle);
		m_players.push_back(player5);
	}*/

	//User* user = CreateEntity<User>({900,555}, {88, 88}, "Perceval", Shape::Circle);
	//user->ShowCollider(true);
	//m_user = user;

	//Application::Get()->GetCamera()->SetIsShow(true);

	//m_playerToCamera = CreateEntity<Player>({ 600, 200 }, { 80, 80 }, "Perceval", Shape::Rectangle);

	CreatePoint("test1", Vector2f(600, 150), Color(0, 0, 255, 255));
	CreateLine("test2", Vector2f(456.0f, 15.0f), Vector2f(400.0f, 7005.0f), Color(0, 0, 255, 255));
	CreateRect("test3", Vector2f(0, 0), Vector2(100, 100), Color(0, 0, 255, 255), false, true);
	CreateFillRect("test4", Vector2f(800.0f, 200.0f), Vector2(64, 32), Color(0, 0, 255, 255));
	GetGeometry<Rectangle>("test4")->SetAngle(700);
	CreateCircle("test5", Vector2f(150, 100), Vector2i(50, 100), Color(0, 0, 255, 255), false, true);
	GetGeometry<Circle>("test5")->SetAngle(64);
	CreateFillCircle("test6", Vector2f(500.0f, 300.0f), Vector2i(240, 64), Color(0, 0, 255, 255));
	GetGeometry<Circle>("test6")->SetAngle(45.0f);
	CreateText("HW", Vector2f(970, 90), "Hello World !", Color(0, 0, 255, 255));
	GetGeometry<Text>("HW")->SetAngle(45.0f);
	CreateFillCircle("88", Vector2f(1000.0f, 500.0f), Vector2i(16, 16), Color(255, 255, 255, 255));	

	float distance = 100.0f;
	Vector2f listenerPos = (Vector2f)Application::Get()->GetWindow()->GetHalfWindowSize();
	Vector2f soundPos = listenerPos + Vector2f(0.0f, -distance);
	sound.GetSound("Monsieur")->SetPosition(soundPos);
	SoundManager::Get()->Play("music1", -1);

	/*int a = 0;
	int b = 1;
	Utils::SwapValue(a, b);

	Vector2f vec1(1.0f, 2.0f);
	Vector2f vec2(3.0f, 4.0f);
	Utils::SwapValue(vec1, vec2);*/

	/*debug.CreatePoint("r", {1100, 300}, Color(255, 0, 0, 255), true);
	debug.CreatePoint("g", { 1150, 250 }, Color(0, 255, 0, 255), true);
	debug.CreatePoint("b", { 1050, 350 }, Color(0, 0, 250, 255), true);*/

	CreateFillCircle("1", Vector2f(25, 25), { 25, 25 }, ColorsNamed::Black, true);
	CreateFillCircle("2", Vector2f(75, 25), { 25, 25 }, ColorsNamed::Blue, true);
	CreateFillCircle("3", Vector2f(125, 25), { 25, 25 }, ColorsNamed::Brown, true);
	CreateFillCircle("4", Vector2f(175, 25), { 25, 25 }, ColorsNamed::DarkBlue, true);
	CreateFillCircle("5", Vector2f(225, 25), { 25, 25 }, ColorsNamed::DarkBrown, true);
	CreateFillCircle("6", Vector2f(275, 25), { 25, 25 }, ColorsNamed::DarkGold, true);
	CreateFillCircle("7", Vector2f(325, 25), { 25, 25 }, ColorsNamed::DarkGray, true);
	CreateFillCircle("8", Vector2f(375, 25), { 25, 25 }, ColorsNamed::DarkGreen, true);
	CreateFillCircle("9", Vector2f(425, 25), { 25, 25 }, ColorsNamed::DarkIndigo, true);
	CreateFillCircle("10", Vector2f(475, 25), { 25, 25 }, ColorsNamed::DarkOrange, true);
	CreateFillCircle("11", Vector2f(525, 25), { 25, 25 }, ColorsNamed::DarkPink, true);
	CreateFillCircle("12", Vector2f(575, 25), { 25, 25 }, ColorsNamed::DarkRed, true);
	CreateFillCircle("13", Vector2f(625, 25), { 25, 25 }, ColorsNamed::DarkSilver, true);
	CreateFillCircle("14", Vector2f(675, 25), { 25, 25 }, ColorsNamed::DarkTurquoise, true);
	CreateFillCircle("15", Vector2f(725, 25), { 25, 25 }, ColorsNamed::DarkViolet, true);
	CreateFillCircle("16", Vector2f(775, 25), { 25, 25 }, ColorsNamed::DarkYellow, true);
	CreateFillCircle("17", Vector2f(825, 25), { 25, 25 }, ColorsNamed::DimGray, true);
	CreateFillCircle("18", Vector2f(875, 25), { 25, 25 }, ColorsNamed::Gold, true);
	CreateFillCircle("19", Vector2f(925, 25), { 25, 25 }, ColorsNamed::Gray, true);
	CreateFillCircle("20", Vector2f(975, 25), { 25, 25 }, ColorsNamed::Green, true);
	CreateFillCircle("21", Vector2f(1025, 25), { 25, 25 }, ColorsNamed::Indigo, true);
	CreateFillCircle("22", Vector2f(1075, 25), { 25, 25 }, ColorsNamed::LightBlue, true);
	CreateFillCircle("23", Vector2f(1125, 25), { 25, 25 }, ColorsNamed::LightBrown, true);
	CreateFillCircle("24", Vector2f(1175, 25), { 25, 25 }, ColorsNamed::LightGold, true);
	CreateFillCircle("25", Vector2f(1225, 25), { 25, 25 }, ColorsNamed::LightGray, true);
	CreateFillCircle("26", Vector2f(25, 75), { 25, 25 }, ColorsNamed::LightGreen, true);
	CreateFillCircle("27", Vector2f(75, 75), { 25, 25 }, ColorsNamed::LightIndigo, true);
	CreateFillCircle("28", Vector2f(125, 75), { 25, 25 }, ColorsNamed::LightOrange, true);
	CreateFillCircle("29", Vector2f(175, 75), { 25, 25 }, ColorsNamed::LightPink, true);
	CreateFillCircle("30", Vector2f(225, 75), { 25, 25 }, ColorsNamed::LightRed, true);
	CreateFillCircle("31", Vector2f(275, 75), { 25, 25 }, ColorsNamed::LightTurquoise, true);
	CreateFillCircle("32", Vector2f(325, 75), { 25, 25 }, ColorsNamed::LightViolet, true);
	CreateFillCircle("33", Vector2f(375, 75), { 25, 25 }, ColorsNamed::LightYellow, true);
	CreateFillCircle("34", Vector2f(425, 75), { 25, 25 }, ColorsNamed::Orange, true);
	CreateFillCircle("35", Vector2f(475, 75), { 25, 25 }, ColorsNamed::Pink, true);
	CreateFillCircle("36", Vector2f(525, 75), { 25, 25 }, ColorsNamed::Red, true);
	CreateFillCircle("37", Vector2f(575, 75), { 25, 25 }, ColorsNamed::Silver, true);
	CreateFillCircle("38", Vector2f(625, 75), { 25, 25 }, ColorsNamed::Turquoise, true);
	CreateFillCircle("39", Vector2f(675, 75), { 25, 25 }, ColorsNamed::VeryLightGray, true);
	CreateFillCircle("40", Vector2f(725, 75), { 25, 25 }, ColorsNamed::VeryLightSilver, true);
	CreateFillCircle("41", Vector2f(775, 75), { 25, 25 }, ColorsNamed::Violet, true);
	CreateFillCircle("42", Vector2f(825, 75), { 25, 25 }, ColorsNamed::White, true);
	CreateFillCircle("43", Vector2f(875, 75), { 25, 25 }, ColorsNamed::Yellow, true);
}

void DemoScene::OnEvent()
{
	if (input.IsGameControllerButtonDown(CONTROLLER_BUTTON_GUIDE))
	{
		ChangeisRunning(false);
	}

	if (input.IsKeyboardDown(K_a))
	{
		int entier = Aleatory::GetInt(1, 100);
		std::cerr << "int " << entier << std::endl;
		float flottant = Aleatory::GetFloat(1.0f, 100.0f);
		std::cerr << "float " << flottant << std::endl;
		char maj = Aleatory::GetCapitalChar();
		std::cerr << "Majuscule char " << maj << std::endl;
		char min = Aleatory::GetTinyChar();
		std::cerr << "Minuscule char " << min << std::endl;
	}

	if (input.IsJoyStickHeld(JOYSTICK_LEFT))
	{
		Vector2f dir = input.GetJoyStickOrientation(JOYSTICK_LEFT);
		std::cerr << "Left Joystick Direction: (" << dir.GetX() << ", " << dir.GetY() << ")" << std::endl;
	}
	if (input.IsJoyStickHeld(JOYSTICK_RIGHT))
	{
		Vector2f dir = input.GetJoyStickOrientation(JOYSTICK_RIGHT);
		std::cerr << "Right Joystick Direction: (" << dir.GetX() << ", " << dir.GetY() << ")" << std::endl;
	}

	if (input.IsMouseDown(BUTTON_LEFT))
	{
		int mouseX = input.GetMouseX();
		int mouseY = input.GetMouseY();
		std::cerr << "Mouse Clicked Left at: (" << mouseX << ", " << mouseY << ")" << std::endl;
		/*if (entitySelected != nullptr)
			entitySelected->Move({ 100,0 });*/
	}
	if (input.IsMouseDown(BUTTON_RIGHT))
	{
		//int mouseX = input.GetMouseX();
		//int mouseY = input.GetMouseY();
		//std::cerr << "Mouse Clicked Right at: (" << mouseX << ", " << mouseY << ")" << std::endl;
		SelectEntity(GetCamera()->ScreenToWorld(Vector2f{ input.GetMouseX(), input.GetMouseY() }));
		//SelectEntity({ input.GetMouseX(), input.GetMouseY() });
	}

	if (input.IsKeyboardDown(K_f))
	{
		if(GetAutoFPS() == false)
		{
			ChangeAutoFPS();
		}
		else
		{
			ChangeAutoFPS();
		}
	}
	if (input.IsKeyboardDown(K_v))
	{
		if (GetAutoFPS() == false)
		{
			int newFPS;
			std::cin >> newFPS;
			SetFPS(newFPS);
		}
	}

	if (input.IsMouseDown(BUTTON_MIDDLE))
	{
		Vector2f mousePosition = GetCamera()->ScreenToWorld(input.GetMousePosition());
		RectangleCollider* collider = new RectangleCollider();
		collider->Initialize(mousePosition, { 10, 10 });
		bool collisionDetected = false;
		for (Player* player : m_players)
		{
			if (Collision::IsColliding(*collider, *player->GetCollider()))
			{
				std::cerr << "Collision detected with player at position: (" << player->GetPosition().GetX() << ", " << player->GetPosition().GetY() << ")" << std::endl;
				player->Destroy();
				collisionDetected = true;
				return;
			}
		}
		if (collisionDetected == false)
		{
			Player* player = CreateEntity<Player>(mousePosition, { 100, 100 }, "Perceval", Shape::Rectangle);
			m_players.push_back(player);
		}
	}

	if (input.IsKeyboardDown(K_p))
	{
		std::cerr << "Playing sound 'Monsieur' " << std::endl;
		sound.Play("Monsieur");
	}

	if (input.IsKeyboardDown(K_KP_DIVIDE))
	{
		sound.SetVolume(0, AudioFormat::MP3);
	}
	if (input.IsKeyboardDown(K_KP_MULTIPLY))
	{
		sound.SetVolume(128, AudioFormat::MP3);
	}
	if (input.IsKeyboardDown(K_KP_PLUS))
	{
		sound.IncreaseVolume(15, AudioFormat::MP3);
	}
	if (input.IsKeyboardDown(K_KP_MINUS))
	{
		sound.DecreaseVolume(15, AudioFormat::MP3);
	}

	float distance = 100.0f;
	Vector2f listenerPos;
	Vector2f soundPos;
	if (sound.GetListener() == nullptr)
	{
		listenerPos = (Vector2f)Application::Get()->GetWindow()->GetHalfWindowSize();
	}
	else
	{
		listenerPos = sound.GetListener()->GetPosition({ 0.5f, 0.5f });
	}
	if (input.IsKeyboardDown(K_KP_8))
	{
		soundPos = listenerPos + Vector2f(0.0f, -distance);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_2))
	{
		soundPos = listenerPos + Vector2f(0.0f, distance);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_4))
	{
		soundPos = listenerPos + Vector2f(-distance, 0.0f);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_6))
	{
		soundPos = listenerPos + Vector2f(distance, 0.0f);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_5))
	{
		soundPos = listenerPos + Vector2f(0.0f, 0.0f);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_7))
	{
		soundPos = listenerPos + Vector2f(-distance, -distance);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_9))
	{
		soundPos = listenerPos + Vector2f(distance, -distance);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_3))
	{
		soundPos = listenerPos + Vector2f(distance, distance);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}
	if (input.IsKeyboardDown(K_KP_1))
	{
		soundPos = listenerPos + Vector2f(-distance, distance);
		sound.SetSoundPosition("Monsieur", { soundPos.GetX(), soundPos.GetY() });
	}

	if (input.IsKeyboardDown(K_EXCLAIM))
	{
		sound.SetPause_Resume_Sound("music1");
	}
	if (input.IsKeyboardDown(K_s))
	{
		sound.Stop("music1");
	}
	if (input.IsKeyboardDown(K_w))
	{
		sound.Play("music1", -1);
	}

#ifdef _DEBUG
	if (input.IsKeyboardDown(K_KP_ENTER))
	{
		sound.DebugVolume();
	}
	if (input.IsKeyboardDown(K_l))
	{
		sound.DebugListenerPosition();
	}

	input.DebugKeys();
	input.DebugControllerButtons();
	input.DebugControllerAxis();
#endif

	if (entitySelected != nullptr)
	{
		if (input.IsKeyboardHeld(K_z))
		{
			std::cerr << "Jump" << std::endl;
			if (entitySelected->GetPhysicsBody())
			{
				std::cerr << "has physics body" << std::endl;
				if (entitySelected->GetIsGrounded())
				{
					std::cerr << "is grounded" << std::endl;
					entitySelected->GetPhysicsBody()->SetLinearVelocity({ entitySelected->GetPhysicsBody()->GetLinearVelocity().GetX(), 0.0f });
					entitySelected->GetPhysicsBody()->ApplyImpulse({ 0.0f, -1000 }, {});
					entitySelected->SetIsGrounded(false);
				}
			}
			else
			{
				entitySelected->SetDirection({ entitySelected->GetDirection().GetX(), -1.0f });
			}
		}
		if (input.IsKeyboardHeld(K_s))
		{
			entitySelected->SetDirection({ entitySelected->GetDirection().GetX(), 1.0f });
		}
		if (input.IsKeyboardHeld(K_q))
		{
			entitySelected->SetDirection({ -1.0f, entitySelected->GetDirection().GetY() });
		}
		if (input.IsKeyboardHeld(K_d))
		{
			entitySelected->SetDirection({ 1.0f, entitySelected->GetDirection().GetY() });
		}
	}
	else
	{
		Camera* camera = GetCamera();
		if (input.IsKeyboardHeld(K_UP))
		{
			camera->Move({ 0.0f, -10.0f });
		}
		if (input.IsKeyboardHeld(K_DOWN))
		{
			camera->Move({ 0.0f, 10.0f });
		}
		if (input.IsKeyboardHeld(K_LEFT))
		{
			camera->Move({ -10.0f, 0.0f });
		}
		if (input.IsKeyboardHeld(K_RIGHT))
		{
			camera->Move({ 10.0f, 0.0f });
		}
	}

	if (input.IsKeyboardHeld(K_KP_MINUS))
	{
		GetGeometry<Text>("HW")->SetColor({ 255, 255, 255, 255 });
		GetGeometry<Text>("HW")->SetText("CACA");
	}
	if (input.IsKeyboardHeld(K_KP_PLUS))
	{
		GetGeometry<Text>("HW")->SetColor({ 255, 0, 0, 255 });
		GetGeometry<Text>("HW")->SetText("GROS CACA");
	}
	if (input.IsKeyboardHeld(K_KP_MULTIPLY))
	{
		GetGeometry<Text>("HW")->SetColor({ 0, 255, 0, 255 });
		GetGeometry<Text>("HW")->SetText("ENORME GROS CACA");
	}

	if (input.IsKeyboardDown(K_F1))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest1");
	}
	if (input.IsKeyboardDown(K_F2))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest2");
	}
	if (input.IsKeyboardDown(K_F3))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest3");
	}
	if (input.IsKeyboardDown(K_F4))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest4");
	}
	if (input.IsKeyboardDown(K_F5))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest5");
	}
	if (input.IsKeyboardDown(K_F6))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest6");
	}
	if (input.IsKeyboardDown(K_F7))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest7");
	}
	if (input.IsKeyboardDown(K_F8))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest8");
	}
	if (input.IsKeyboardDown(K_F9))
	{
		MapTiledManager::Get()->SetCurrentMap("MapTest9");
	}

	if (input.IsKeyboardDown(K_1))
	{
		GetCamera()->SetZoom(0.2f);
	}
	if (input.IsKeyboardDown(K_2))
	{
		GetCamera()->SetZoom(0.4f);
	}
	if (input.IsKeyboardDown(K_3))
	{
		GetCamera()->SetZoom(0.8f);
	}
	if (input.IsKeyboardDown(K_4))
	{
		GetCamera()->SetZoom(1.0f);
	}
	if (input.IsKeyboardDown(K_5))
	{
		GetCamera()->SetZoom(1.5f);
	}
	if (input.IsKeyboardDown(K_6))
	{
		GetCamera()->SetZoom(2.0f);
	}
	if (input.IsKeyboardDown(K_7))
	{
		GetCamera()->SetZoom(3.0f);
	}
	if (input.IsKeyboardDown(K_8))
	{
		GetCamera()->SetZoom(4.0f);
	}
	if (input.IsKeyboardDown(K_9))
	{
		GetCamera()->SetZoom(5.0f);
	}
	if (input.IsMouseHeld(BUTTON_X1))
	{
		std::cerr << "Mouse X1 Button Held" << std::endl;
	}
	if (input.IsMouseHeld(BUTTON_X2))
	{
		std::cerr << "Mouse X2 Button Held" << std::endl;
	}
	if (input.IsMouseScrollDown(SCROLL_UP))
	{
		Vector2f mouseScreen = input.GetMousePosition();
		Vector2f before = GetCamera()->ScreenToWorld(mouseScreen);

		GetCamera()->ZoomIn(0.1f);

		Vector2f after = GetCamera()->ScreenToWorld(mouseScreen);
		Vector2f delta = before - after;

		GetCamera()->SetPosition(GetCamera()->GetPosition() + delta);
		std::cerr << "Mouse Scrolled Up" << std::endl;
	}

	if (input.IsMouseScrollDown(SCROLL_DOWN))
	{
		Vector2f mouseScreen = input.GetMousePosition();
		Vector2f before = GetCamera()->ScreenToWorld(mouseScreen);

		GetCamera()->ZoomOut(0.1f);

		Vector2f after = GetCamera()->ScreenToWorld(mouseScreen);
		Vector2f delta = before - after;

		GetCamera()->SetPosition(GetCamera()->GetPosition() + delta);
		std::cerr << "Mouse Scrolled Down" << std::endl;
	}
	if (input.IsKeyboardDown(K_ESCAPE))
	{
		entitySelected = nullptr;
	}

	if (input.IsKeyboardDown(K_TAB))
	{
		//SceneManager::Get()->ClearScenesInStack();
		SceneManager::Get()->PushScene("test");
	}
}

void DemoScene::OnUpdate()
{
	//TestVélocité();
	std::cout << "Player1 = " <<m_player1->GetIsGrounded() << std::endl;
	std::cout << "Player2 = " <<m_player2->GetIsGrounded() << std::endl;
	Vector2f positionSound = sound.GetSoundPosition("Monsieur");

	GetGeometry<Circle>("88")->SetPosition(positionSound);

	if (entitySelected != nullptr)
	{
		SoundManager::Get()->SetListener(entitySelected);

		Vector2f camTargetPos = entitySelected->GetPosition();
		GetCamera()->SetPosition(camTargetPos);


		//std::cerr << "player pos : " << camTargetPos.GetX() << " " << camTargetPos.GetY() << std::endl;

		/*debug.Get()->GetRect("Camera")->SetDimension(GetCamera()->GetDimension());
		debug.Get()->GetRect("Camera")->SetPosition(camTargetPos);*/
	}
	/*std::size_t maxSize = Utils::Type_Size<InputManager>();
	std::cerr << "Size of Type: " << maxSize << " bytes" << std::endl;*/

	/*Vector2f camTargetPos = m_playerToCamera->GetPosition();
	GetCamera()->SetPosition(camTargetPos);*/
	
	/*debug.Get()->destroyPoint("r");
	debug.Get()->destroyPoint("g");
	debug.Get()->destroyPoint("b");*/
}

void DemoScene::OnSleep()
{
	SetIsVisible(false);
	SetSleepEvent(true);
	SetSleepUpdate(true);
}

void DemoScene::OnWake()
{
	SetIsVisible(true);
	SetSleepEvent(false);
	SetSleepUpdate(false);
}

void DemoScene::OnDestroy()
{
	/*for (Player* player : m_players)
	{
		player->Destroy();
	}
	if (m_user != nullptr)
	{
		m_user->Destroy();
		m_user = nullptr;
	}
	if (entitySelected != nullptr)
	{
		entitySelected = nullptr;
	}
	if (lastEntitySelected != nullptr)
	{
		lastEntitySelected = nullptr;
	}*/
	m_players.clear();
	debug.ClearAll();
	GetEntities().clear();
}

void DemoScene::TestVélocité()
{
	for (Player* player : m_players)
	{
		if (player->GetTag() == 64 && player->GetPhysicsBody())
		{
			Vector2f vel = player->GetPhysicsBody()->GetLinearVelocity();
			std::cout << "Velocity : X=" << vel.GetX()
				<< " Y=" << vel.GetY() << std::endl;
		}
	}
}

void DemoScene::SelectEntity(Vector2f p_position)
{
	RectangleCollider* collider = new RectangleCollider();
	collider->Initialize(p_position, { 10, 10 });
	for (Player* player : m_players)
	{
		if (Collision::IsColliding(*collider, *player->GetCollider()))
		{
			if (entitySelected != nullptr)
			{
				lastEntitySelected = entitySelected;
			}
			entitySelected = player;
			return;
		}
	}
}