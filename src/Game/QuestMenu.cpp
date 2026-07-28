#include "pch.h"
#include "QuestMenu.h"
#include "Application.h"


QuestMenu::QuestMenu(int p_slotCount)
{
	m_slots.reserve(p_slotCount + 10);
}

QuestMenu::~QuestMenu()
{


}

void QuestMenu::OnInitialize()
{
	SetDrawLayer(INT_MAX - 2);

	Application* app = Application::Get();
	Window* window = app->GetWindow();

	float centerX = window->GetWindowSize().GetX() / 2.0f;
	float centerY = window->GetWindowSize().GetY() / 2.0f;

	int marginOut = 20; //marge pour les bord du menu

	// create required slot
	Scene* scene = GetScene();

	////Title
	//QuestMenuSlot* title = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 380, 100 }, "button", Shape::Rectangle, true);
	//title->GetSprite()->ResizeStretched(title->GetDimension());
	//title->SyncSprites();
	//title->SetPosition(GetPosition({ 0.0f, 0.0f }) + Vector2f{marginOut * 1.5f, marginOut * 2.5f}, { 0.0f, 0.0f });
	//title->GetCollider()->SetIsTrigger(true);
	//title->GetCollider()->SetShow(true);
	//m_slots.push_back(title);

	////Reward 
	//QuestMenuSlot* reward = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 380, 120 }, "button", Shape::Rectangle, true);
	//reward->GetSprite()->ResizeStretched(reward->GetDimension());
	//reward->SyncSprites();
	//reward->SetPosition(title->GetPosition({ 0.5f, 1.0f }) + Vector2f{ 0.0f, (float)marginOut / 2 }, { 0.5f, 0.0f });
	//reward->GetCollider()->SetIsTrigger(true);
	//reward->GetCollider()->SetShow(true);
	//m_slots.push_back(reward);

	////History
	//QuestMenuSlot* history = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 380, (int)std::ceil(reward->GetPosition({0.5f, 1.0f}).GetY() - title->GetPosition({0.5f, 0.0f}).GetY()) + 6 }, "button", Shape::Rectangle, true);
	//history->GetSprite()->ResizeStretched(history->GetDimension());
	//history->SyncSprites();
	//history->SetPosition(title->GetPosition({ 1.0f, 0.0f }) + Vector2f{ (float)marginOut, 0.0f }, { 0.0f, 0.0f });
	//history->GetCollider()->SetIsTrigger(true);
	//history->GetCollider()->SetShow(true);
	//m_slots.push_back(history);

	////Requiquement
	//QuestMenuSlot* requiquement = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 780, 160 }, "button", Shape::Rectangle, true);
	//requiquement->GetSprite()->ResizeStretched(requiquement->GetDimension());
	//requiquement->SyncSprites();
	//requiquement->SetPosition({ history->GetPosition({1.0f, 0.5f}).GetX() - 390, history->GetPosition({0.5f, 1.0f}).GetY() + marginOut * 0.34f }, { 0.5f, 0.0f });
	//requiquement->GetCollider()->SetIsTrigger(true);
	//requiquement->GetCollider()->SetShow(true);
	//m_slots.push_back(requiquement);

	////Follow Quest
	//QuestMenuSlot* followQuest = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 100, 69 }, "button", Shape::Rectangle, true);
	//followQuest->GetSprite()->ResizeStretched(followQuest->GetDimension());
	//followQuest->SyncSprites();
	//followQuest->SetPosition(requiquement->GetPosition({ 0.25f, 1.f }) + Vector2f{ 0.0f, (float)marginOut }, { 0.0f, 0.0f });
	//followQuest->GetCollider()->SetIsTrigger(true);
	//followQuest->GetCollider()->SetShow(true);
	//m_slots.push_back(followQuest);

	////Unfollow Quest
	//QuestMenuSlot* unfollowQuest = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 100, 69 }, "button", Shape::Rectangle, true);
	//unfollowQuest->GetSprite()->ResizeStretched(unfollowQuest->GetDimension());
	//unfollowQuest->SyncSprites();
	//unfollowQuest->SetPosition(requiquement->GetPosition({ 0.75f, 1.0f }) + Vector2f{ 0.0f,(float)marginOut }, { 1.0f, 0.0f });
	//unfollowQuest->GetCollider()->SetIsTrigger(true);
	//unfollowQuest->GetCollider()->SetShow(true);
	//m_slots.push_back(unfollowQuest);

	////Next Quest
	//QuestMenuSlot* nextQuest = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 80, 67 }, "button", Shape::Rectangle, true);
	//nextQuest->GetSprite()->ResizeStretched(nextQuest->GetDimension());
	//nextQuest->SyncSprites();
	//nextQuest->SetPosition(GetPosition({ 1.0f, 1.0f }) + Vector2f{ -(float)marginOut * 2, -(float)marginOut * 2.5f }, { 1.0f, 1.0f });
	//nextQuest->GetCollider()->SetIsTrigger(true);
	//nextQuest->GetCollider()->SetShow(true);

	////Previous Quest
	//QuestMenuSlot* previousQuest = scene->CreateEntity<QuestMenuSlot>({ 0.0f, 0.0f }, { 80, 67 }, "button", Shape::Rectangle, true);
	//previousQuest->GetSprite()->ResizeStretched(previousQuest->GetDimension());
	//previousQuest->SyncSprites();
	//previousQuest->SetPosition({ requiquement->GetPosition({1.0f, 0.5f}).GetX() + marginOut, nextQuest->GetPosition({0.5f, 0.0f}).GetY() }, { 0.0f, 0.0f });
	//previousQuest->GetCollider()->SetIsTrigger(true);
	//previousQuest->GetCollider()->SetShow(true);

	SetVisible(false);
}

void QuestMenu::SyncQuests(const std::vector<Quest*>& p_questVector)
{

}

void QuestMenu::SetVisible(bool p_visible)
{
	SetIsVisible(p_visible);

	for (QuestMenuSlot* slot : m_slots)
	{
		slot->SetIsVisible(p_visible);
	}
}

void QuestMenu::Open()
{
	m_isOpen = true;
	SetVisible(true);
}

void QuestMenu::Close()
{
	m_isOpen = false;
	SetVisible(false);
}
void QuestMenu::Toggle()
{
	if (m_isOpen)
		Close();
	else
		Open();
}
