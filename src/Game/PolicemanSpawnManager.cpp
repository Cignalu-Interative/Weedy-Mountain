#include "pch.h"
#include "PolicemanSpawnManager.h"
#include "SceneManager.h"
#include "VillageScene.h"
#include "GameData.h"

PolicemanSpawnManager& PolicemanSpawnManager::Get()
{
	static PolicemanSpawnManager Instance;
	return Instance;
}

void PolicemanSpawnManager::SpawnPoliceman()
{
	SceneManager* sm = SceneManager::Get();

	m_policeman = sm->GetScene("village")->CreateEntity<Policeman>({1120, 640}, {16, 32}, "player", Shape::Rectangle);
	m_policeman->InitialisePhysicsBody(ShapeBodyPhysics::Kinematic);

	m_policeman2 = sm->GetScene("village")->CreateEntity<Policeman>({ 1120, 640 }, { 16, 32 }, "player", Shape::Rectangle);
	m_policeman2->InitialisePhysicsBody(ShapeBodyPhysics::Kinematic);

	m_policeman3 = sm->GetScene("village")->CreateEntity<Policeman>({ 1120, 640 }, { 16, 32 }, "player", Shape::Rectangle);
	m_policeman3->InitialisePhysicsBody(ShapeBodyPhysics::Kinematic);

	m_policeman4 = sm->GetScene("village")->CreateEntity<Policeman>({ 1120, 640 }, { 16, 32 }, "player", Shape::Rectangle);
	m_policeman4->InitialisePhysicsBody(ShapeBodyPhysics::Kinematic);

	m_policeman5 = sm->GetScene("village")->CreateEntity<Policeman>({ 1120, 640 }, { 16, 32 }, "player", Shape::Rectangle);
	m_policeman5->InitialisePhysicsBody(ShapeBodyPhysics::Kinematic);

	m_isDespawn = false;
}

void PolicemanSpawnManager::DespawnPoliceman()
{
	m_policeman->Destroy();
	m_policeman2->Destroy();
	m_policeman3->Destroy();
	m_policeman4->Destroy();
	m_policeman5->Destroy();

	m_isDespawn = true;
}

void PolicemanSpawnManager::Update()
{
	GameData gd = GameData::Get();

	if (gd.GetDayName() == "Mercredi" || gd.GetDayName() == "Jeudi" || gd.GetDayName() == "Dimanche")
	{
		return;
	}

	if (gd.GetHour() >= 20 && m_policeman->ToDestroy() == false)
	{
		DespawnPoliceman();
		return;
	}

	if (gd.GetHour() >= 9 && m_isDespawn)
	{
		SpawnPoliceman();
	}
}
