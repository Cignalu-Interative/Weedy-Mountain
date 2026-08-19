#include "pch.h"
#include "VillageScene.h"
#include "2DPendentLib/InputManager.h"
#include "2DPendentLib/Utils.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentEngine/MapTiledManager.h"
#include "2DPendentEngine/ThreadManager.h"
#include "2DPendentLib/FontManager.h"
#include "SpriteContainer.h"

#include "Player.h"
#include "GameData.h"
#include "PNJ.h"
#include "shopPNJ.h"

#include "GameScene.h"
#include "Ui/UiStats.h"
#include "Ui/UiMoney.h"
#include "Ui/UiCash.h"
#include "Ui/UiDate.h"
#include "Ui/UiTime.h"
#include "Ui/UiJobs.h"
#include "Ui/UiGatherer.h"
#include "Ui/UiShop.h"
#include "UiDealerShop.h"
#include "ATM.h"

#include "2DPendentLib/ObjectGroupTiled.h"
#include "2DPendentLib/ObjectTiled.h"

#include "QuestManager.h"

void VillageScene::OnInitialize()
{
	AssetManager::Get()->AddTextureByRepertory("../../res/Game/Assets/Textures/Textures_Repertory.json");
	MapTiledManager* mm = MapTiledManager::Get();
	mm->AddMaps("../../res/Game/Assets/Maps/Maps_Repertory.json");
	mm->SetCurrentMap("Village");

	std::vector<ObjectTiled*> a = mm->GetCurrentMap()->GetObjectGroupByName("Object")->GetObjectTiledByName("PlayerSpawn"); 

	for (ObjectTiled* o : a)
	{
		m_spawnPoint = o->GetObjectPosition();
	}

	GetCamera()->SetPosition(m_spawnPoint);

	m_player = CreateEntity<Player>(m_spawnPoint, {16,32}, "player", Shape::Rectangle);
	m_player->GetCollider()->SetDimension({ 14,14 });
	m_player->GetCollider()->SetOffset({ 0,9 });
	m_player->SetRigidBody(true);
	m_player->SetTag(1);
	m_player->SetDrawLayer(2);
	m_player->GetCollider()->SetLayer(PLAYER);
	m_player->AddCollider("actionBox", { 16,16 }, Shape::Rectangle, { 0,0 }, true);
	m_player->AddCollider("interact", { 80,80 }, Shape::Circle, { 0,0 }, true);
	m_player->ShowCollider(true);
	m_player->InitialisePhysicsBody(ShapeBodyPhysics::Dynamic);
	m_player->GetPhysicsBody()->SetGravityEnabled(false);
	m_player->GetPhysicsBody()->SetLinearDamping(0.f);
	m_player->GetPhysicsBody()->SetRestitution(0.f);

	m_shopPNJ = CreateEntity<shopPNJ>({ m_spawnPoint + Vector2f{200,0} }, { 8,32 }, "player", Shape::Rectangle);
	m_PNJ = CreateEntity<PNJ>({ m_spawnPoint + Vector2f{200,-100} }, { 8,32 }, "player", Shape::Rectangle);
	m_drugDealer = CreateEntity<shopPNJ>({ m_spawnPoint + Vector2f{200,-200} }, { 8,32 }, "player", Shape::Rectangle);

	m_PNJ->SetOnFinishFunction([this]
		{
			m_player->UnlockInputs();

			Quest* quest = QuestManager::Get()->GetQuest("Vendre Recolte");
			if (quest == nullptr)
				return;

			if (quest->IsBlocked())
			{
				QuestManager::Get()->SetQuestState("Vendre Recolte", QuestState::InProgress);
			}
			else if (quest->IsFinished())
			{
				std::cerr << "Quest finished" << std::endl;
				//m_PNJ->Talk("Merci beaucoup pour ton aide, tu es vraiment un bon voisin ! Je te souhaite une bonne continuation !");
			}
			else
			{
				//m_PNJ->Talk("Tu n'as pas encore termine la quete, continue a chercher les objets dont j'ai besoin !");
			}
		});

	m_drugDealer->SetOnFinishFunction([this] {
		if (GameData::Get().GetHour() < 18)
		{
			m_player->UnlockInputs();
			return;
		}
		if (GameData::Get().GetHour() > 20)
		{
			m_player->UnlockInputs();
			return;
		}

		m_dealerShop->Open();
		}
	);
	m_shopPNJ->SetOnFinishFunction([this] {
		m_shop->Open(); 
		}
	);

	m_shopPNJ->GetInventory()->AddItem("ITEM_WOOD", 64, false);
	m_shopPNJ->GetInventory()->AddItem("ITEM_FIBER", 64, false);


	m_drugDealer->GetInventory()->AddItem("ITEM_WEED", 64, false);


	m_shop = CreateEntity<UiShop>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 1000, 600 }, "InventoryWeedyMountain", Shape::Rectangle, true);
	m_shop->GetSprite()->SetDimension(m_shop->GetDimension());
	m_shop->SyncSprites();
	m_shop->SetShopInventory(m_shopPNJ->GetInventory());
	m_shop->SetIsVisible(false);

	m_dealerShop = CreateEntity<UiDealerShop>({ GetWindowWidth() / 2.f, GetWindowHeight() / 2.f }, { 1000, 600 }, "InventoryWeedyMountain", Shape::Rectangle, true);
	m_dealerShop->GetSprite()->SetDimension(m_dealerShop->GetDimension());
	m_dealerShop->SyncSprites();
	m_dealerShop->SetShopInventory(m_drugDealer->GetInventory());
	m_dealerShop->SetIsVisible(false);

	
	m_atm = CreateEntity<ATM>(m_spawnPoint + Vector2f(350, -100), { 32, 46 }, "ATM", Shape::Rectangle);


	GetCamera()->SetFollowedEntity(m_player);
	Application::Get()->SetClampCameraToCurrentMap(true);

	
}

void VillageScene::OnEvent()
{
	InputManager* im = InputManager::Get();

	if (im->IsKeyboardDown(K_X))
	{
		SceneManager::Get()->PushScene("game");
	}

	if (im->IsKeyboardDown(K_A))
		GetCamera()->SetZoom(5);

	if (im->IsKeyboardDown(K_E) && (!m_shopPNJ->IsInPlayerZone() && !m_PNJ->IsInPlayerZone()))
		GetCamera()->SetZoom(1);

	if (im->IsKeyboardDown(K_M))
		m_player->TakeDamage(5);
	if (im->IsKeyboardDown(K_U))
		m_player->Heal(5);

	if (InputManager::Get()->IsKeyboardDown(K_E) && m_shopPNJ->IsInPlayerZone())
	{
		m_player->LockInputs();
		m_shopPNJ->Talk("Bonjour, bienvenue dans la boutique! Que puis-je faire pour vous aujourd'hui ?");
	}

	if (InputManager::Get()->IsKeyboardDown(K_E) && m_PNJ->IsInPlayerZone())
	{
		m_player->LockInputs();
		m_PNJ->Talk("Salut ! Moi c'est Richard, ca fait maintenant un moment que j'habite ici et j'ai entendu dire que tu venais d'arriver dans le village, je voulais te souhaiter la bienvenue. Surtout si tu as besoin de quoi que ce soit n'hesite pas ! ");
	}

	if (InputManager::Get()->IsKeyboardDown(K_E) && m_drugDealer->IsInPlayerZone())
	{
		m_player->LockInputs();
		m_drugDealer->Talk("Salut ! Soit discret, jai peut etre ce qu'il te faut . ");
	}

	if (InputManager::Get()->IsKeyboardDown(K_ESCAPE) && m_shop->GetIsVisible())
	{
		m_shop->Close();
		m_player->UnlockInputs();
	}
	if (InputManager::Get()->IsKeyboardDown(K_ESCAPE) && m_dealerShop->GetIsVisible())
	{
		m_dealerShop->Close();
		m_player->UnlockInputs();
	}
	if (InputManager::Get()->IsKeyboardDown(K_E) && m_atm->IsInPlayerZone() && !m_atm->IsOpen())
	{
		m_atm->Toogle();
		m_player->LockInputs();
	}
	if (InputManager::Get()->IsKeyboardDown(K_ESCAPE) && m_atm->IsOpen())
	{
		m_atm->Toogle();
		m_player->UnlockInputs();
	}
}

void VillageScene::OnUpdate()
{
}

void VillageScene::OnDestroy()
{
}

void VillageScene::OnSleep()
{
	SetSleepDraw(true);
	SetSleepEvent(true);
}

void VillageScene::OnWake()
{
	GameData::Get().SetCurrentScene(CurrentScene::VILLAGE);

	MapTiledManager* mm = MapTiledManager::Get();
	mm->SetCurrentMap("Village");


	m_player->SetPosition(m_spawnPoint);
	GetCamera()->SetPosition(m_player->GetPosition());
	GetCamera()->SetFollowedEntity(m_player);
	SetSleepDraw(false);
	SetSleepEvent(false);
}

void VillageScene::OnDraw()
{
}
