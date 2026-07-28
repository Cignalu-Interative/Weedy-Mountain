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

	//Title
	QuestMenuSlot* title = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 380, 100 }, "button", "title", "", true);
	title->GetSprite()->ResizeStretched(title->GetDimension());
	title->SyncSprites();
	title->SetPosition(GetPosition({ 0.0f, 0.0f }) + Vector2f{ marginOut * 1.5f, marginOut * 2.5f }, { 0.0f, 0.0f });
	title->GetButtonText()->SetPosition(title->GetPosition());
	title->GetCollider()->SetIsTrigger(true);
	//title->GetCollider()->SetShow(true);
	m_slots.push_back(title);
	
	

	//Reward
	QuestMenuSlot* reward = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 380, 120 }, "button", "reward", "", true);
	reward->GetSprite()->ResizeStretched(reward->GetDimension());
	reward->SyncSprites();
	reward->SetPosition(title->GetPosition({ 0.5f, 1.0f }) + Vector2f{ 0.0f, (float)marginOut / 2 }, { 0.5f, 0.0f });
	reward->GetButtonText()->SetPosition(reward->GetPosition());
	reward->GetCollider()->SetIsTrigger(true);
	//reward->GetCollider()->SetShow(true);
	m_slots.push_back(reward);

	//History
	QuestMenuSlot* history = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 380, (int)std::ceil(reward->GetPosition({0.5f, 1.0f}).GetY() - title->GetPosition({0.5f, 0.0f}).GetY()) + 6 }, "button", "history", "", true);
	history->GetSprite()->ResizeStretched(history->GetDimension());
	history->SyncSprites();
	history->SetPosition(title->GetPosition({ 1.0f, 0.0f }) + Vector2f{ (float)marginOut, 0.0f }, { 0.0f, 0.0f });
	history->GetButtonText()->SetPosition(history->GetPosition());
	history->GetCollider()->SetIsTrigger(true);
	//history->GetCollider()->SetShow(true);
	m_slots.push_back(history);

	//Requiquement
	QuestMenuSlot* requiquement = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 780, 160 }, "button", "requiquement", "", true);
	requiquement->GetSprite()->ResizeStretched(requiquement->GetDimension());
	requiquement->SyncSprites();
	requiquement->SetPosition({ history->GetPosition({1.0f, 0.5f}).GetX() - 390, history->GetPosition({0.5f, 1.0f}).GetY() + marginOut * 0.34f }, { 0.5f, 0.0f });
	requiquement->GetButtonText()->SetPosition(requiquement->GetPosition());
	requiquement->GetCollider()->SetIsTrigger(true);
	//requiquement->GetCollider()->SetShow(true);
	m_slots.push_back(requiquement);

	//Follow Quest
	QuestMenuSlot* followQuest = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 160, 69 }, "button", "followQuest", "follow Quest", true);
	followQuest->GetSprite()->ResizeStretched(followQuest->GetDimension());
	followQuest->SyncSprites();
	followQuest->SetPosition(requiquement->GetPosition({ 0.25f, 1.f }) + Vector2f{ 0.0f, (float)marginOut }, { 0.0f, 0.0f });
	followQuest->GetButtonText()->SetPosition(followQuest->GetPosition());
	followQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	followQuest->GetCollider()->SetIsTrigger(true);
	//followQuest->GetCollider()->SetShow(true);
	m_slots.push_back(followQuest);

	//Unfollow Quest
	QuestMenuSlot* unfollowQuest = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 160, 69 }, "button", "unfollowQuest", "unfollow Quest", true);
	unfollowQuest->GetSprite()->ResizeStretched(unfollowQuest->GetDimension());
	unfollowQuest->SyncSprites();
	unfollowQuest->SetPosition(requiquement->GetPosition({ 0.75f, 1.0f }) + Vector2f{ 0.0f,(float)marginOut - 1 }, { 1.0f, 0.0f });
	unfollowQuest->GetButtonText()->SetPosition(unfollowQuest->GetPosition());
	unfollowQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	unfollowQuest->GetCollider()->SetIsTrigger(true);
	//unfollowQuest->GetCollider()->SetShow(true);
	m_slots.push_back(unfollowQuest);

	//Next Quest
	QuestMenuSlot* nextQuest = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 100, 67 }, "button", "nextQuest", "->", true);
	nextQuest->GetSprite()->ResizeStretched(nextQuest->GetDimension());
	nextQuest->SyncSprites();
	nextQuest->SetPosition({ GetPosition({ 1.0f, 0.5f }).GetX() + -(float)marginOut * 2, unfollowQuest->GetPosition({0.5f, 1.0f}).GetY()}, {1.0f, 1.0f});
	nextQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	nextQuest->GetButtonText()->SetFontSize(42);
	nextQuest->GetButtonText()->SetPosition(nextQuest->GetPosition());
	nextQuest->GetCollider()->SetIsTrigger(true);
	//nextQuest->GetCollider()->SetShow(true);
	m_slots.push_back(nextQuest);

	//Previous Quest
	QuestMenuSlot* previousQuest = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 100, 67 }, "button", "previousQuest", "<-", true);
	previousQuest->GetSprite()->ResizeStretched(previousQuest->GetDimension());
	previousQuest->SyncSprites();
	previousQuest->SetPosition({ requiquement->GetPosition({1.0f, 0.5f}).GetX() + marginOut, nextQuest->GetPosition({0.5f, 0.0f}).GetY() }, { 0.0f, 0.0f });
	previousQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	previousQuest->GetButtonText()->SetFontSize(42);
	previousQuest->GetButtonText()->SetPosition(previousQuest->GetPosition());
	previousQuest->GetCollider()->SetIsTrigger(true);
	//previousQuest->GetCollider()->SetShow(true);
	m_slots.push_back(previousQuest);

	//List of Quest
	QuestMenuSlot* listOfQuest = (QuestMenuSlot*)scene->CreateButton({ 0.0f, 0.0f }, { 220, ((int)(requiquement->GetPosition({0.5f, 1.0f}).GetY() - history->GetPosition({0.5f, 0.0f}).GetY())) + 11 }, "buttonHover", "listOfQuest", "", true);
	listOfQuest->GetSprite()->ResizeStretched(listOfQuest->GetDimension());
	listOfQuest->SyncSprites();
	listOfQuest->SetPosition(history->GetPosition({1.0f, 0.0f}) + Vector2f{(float)marginOut, 0.0f}, {0.0f, 0.0f});
	listOfQuest->GetButtonText()->SetPosition(listOfQuest->GetPosition());
	listOfQuest->GetCollider()->SetIsTrigger(true);
	//listOfQuest->GetCollider()->SetShow(true);
	m_slots.push_back(listOfQuest);

	SetVisible(false);
}

void QuestMenu::SyncQuests(const std::vector<Quest*>* p_questVector)
{
	m_quests.clear();
	float margin = 10;
	int count = 0;
	QuestMenuSlot* listOfQuest = GetSlot("listOfQuest");
	for (Quest* quest : *p_questVector)
	{
		QuestMenuSlot* questSlot = (QuestMenuSlot*)m_scene->CreateButton({ 0.0f, 0.0f }, { 180, 50 }, "button", quest->GetName(), quest->GetName(), true);
		questSlot->GetSprite()->ResizeStretched(questSlot->GetDimension());
		questSlot->SyncSprites();
		questSlot->SetPosition(listOfQuest->GetPosition({0.5f, 0.08f}) + Vector2f{ 0.0f, margin + (questSlot->GetDimension().GetY() * count)}, {0.5f, 0.0f});
		questSlot->GetButtonText()->SetColor(ColorsNamed::DarkIndigo);
		questSlot->GetButtonText()->SetFontSize(24);
		questSlot->GetButtonText()->SetPosition(questSlot->GetPosition());
		questSlot->GetCollider()->SetIsTrigger(true);
		questSlot->SetOnLeftClick([this, questSlot] {DisplayQuest(questSlot); });
		//questSlot->GetCollider()->SetShow(true);
		m_slots.push_back(questSlot);
		m_quests.push_back(quest);
		count++;
	}
}

void QuestMenu::SetVisible(bool p_visible)
{
	SetIsVisible(p_visible);

	for (QuestMenuSlot* slot : m_slots)
	{
		slot->SetIsVisible(p_visible);
	}
}

void QuestMenu::Open(std::vector<Quest*>* p_quest)
{
	m_isOpen = true;
	SyncQuests(p_quest);
	SetVisible(true);
}

void QuestMenu::Close()
{
	m_isOpen = false;
	SetVisible(false);
	m_selectedQuest = nullptr;
}
void QuestMenu::Toggle(std::vector<Quest*>* p_quest)
{
	if (m_isOpen)
		Close();
	else
		Open(p_quest);
}

void QuestMenu::OnUpdate()
{
	if (!m_isOpen)
		return;

	if (m_quests.empty())
	{
		GetSlot("listOfQuest")->GetButtonText()->SetColor(ColorsNamed::Gray);
		GetSlot("listOfQuest")->GetButtonText()->SetFontSize(42);
		GetSlot("listOfQuest")->GetButtonText()->SetText("No Quest");
		GetSlot("listOfQuest")->GetButtonText()->SetPosition(GetSlot("listOfQuest")->GetPosition());
	}
}

QuestMenuSlot* QuestMenu::GetSlot(const std::string& p_id)
{
	for (QuestMenuSlot* slot : m_slots)
	{
		if (slot->GetTextID() == p_id)
			return slot;
	}
	return nullptr;
}

void QuestMenu::DisplayQuest(QuestMenuSlot* p_quest)
{
	for (Quest* quest : m_quests)
	{
		if (quest->GetName() == p_quest->GetTextID())
		{
			m_selectedQuest = quest;
			break;
		}
	}
}
