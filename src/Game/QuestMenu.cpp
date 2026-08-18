#include "pch.h"
#include "QuestMenu.h"
#include "Application.h"
#include "2DPendentLib/Utils.h"
#include "QuestManager.h"
#include "HUDScene.h"

QuestMenu::QuestMenu(int p_slotCount)
{
	m_slots.reserve(p_slotCount + 10);
	m_quests.reserve(10);
}

QuestMenu::~QuestMenu()
{
	/*for (QuestMenuSlot* slot : m_slots)
	{
		if (slot != nullptr)
		{
			slot->Destroy();
		}
	}*/
	/*for (Quest* quest : m_quests)
	{
		if (quest != nullptr)
		{
			delete quest;
		}
	}*/

	if (m_slots.empty() == false)
	{
		m_slots.clear();
	}
	if (m_quests.empty() == false)
	{
		m_quests.clear();
	}
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
	QuestMenuSlot* title = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 380, 100 }, "button", "title", "", true);
	title->GetSprite()->ResizeStretched(title->GetDimension());
	title->SyncSprites();
	title->SetPosition(GetPosition({ 0.0f, 0.0f }) + Vector2f{ marginOut * 1.5f, marginOut * 2.5f }, { 0.0f, 0.0f });
	title->GetButtonText()->SetPosition(title->GetPosition());
	title->GetCollider()->SetIsTrigger(true);
	//title->GetCollider()->SetShow(true);
	m_slots.push_back(title);

	//Reward
	QuestMenuSlot* reward = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 380, 120 }, "button", "reward", "", true);
	reward->GetSprite()->ResizeStretched(reward->GetDimension());
	reward->SyncSprites();
	reward->SetPosition(title->GetPosition({ 0.5f, 1.0f }) + Vector2f{ 0.0f, (float)marginOut / 2 }, { 0.5f, 0.0f });
	reward->GetButtonText()->SetPosition(reward->GetPosition());
	reward->GetCollider()->SetIsTrigger(true);
	//reward->GetCollider()->SetShow(true);
	m_slots.push_back(reward);


	//Text Reward
	scene->CreateText("textReward", reward->GetPosition({ 0.5f, 0.0f }), "REWARDS", ColorsNamed::Black, true);
	m_rewardText = scene->GetGeometry<Text>("textReward");
	m_rewardText->SetFontSize(36);
	m_rewardText->SetPosition(reward->GetPosition({ 0.5f, 0.2f }));

	//History
	QuestMenuSlot* history = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 380, (int)std::ceil(reward->GetPosition({0.5f, 1.0f}).GetY() - title->GetPosition({0.5f, 0.0f}).GetY()) + 6 }, "button", "history", "", true);
	history->GetSprite()->ResizeStretched(history->GetDimension());
	history->SyncSprites();
	history->SetPosition(title->GetPosition({ 1.0f, 0.0f }) + Vector2f{ (float)marginOut, 0.0f }, { 0.0f, 0.0f });
	history->GetButtonText()->SetPosition(history->GetPosition());
	history->GetCollider()->SetIsTrigger(true);
	//history->GetCollider()->SetShow(true);
	m_slots.push_back(history);

	//Requiquement
	QuestMenuSlot* requiquement = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 780, 160 }, "button", "requiquement", "", true);
	requiquement->GetSprite()->ResizeStretched(requiquement->GetDimension());
	requiquement->SyncSprites();
	requiquement->SetPosition({ history->GetPosition({1.0f, 0.5f}).GetX() - 390, history->GetPosition({0.5f, 1.0f}).GetY() + marginOut * 0.34f }, { 0.5f, 0.0f });
	requiquement->GetButtonText()->SetPosition(requiquement->GetPosition());
	requiquement->GetCollider()->SetIsTrigger(true);
	//requiquement->GetCollider()->SetShow(true);
	m_slots.push_back(requiquement);

	//Text Requiquement
	scene->CreateText("textRequiquement", requiquement->GetPosition({ 0.5f, 0.0f }), "REQUIREMENTS", ColorsNamed::Black, true);
	m_requirementText = scene->GetGeometry<Text>("textRequiquement");
	m_requirementText->SetFontSize(36);
	m_requirementText->SetPosition(requiquement->GetPosition({ 0.5f, 0.2f }));

	//Follow Quest
	QuestMenuSlot* followQuest = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 160, 69 }, "button", "followQuest", "follow Quest", true);
	followQuest->GetSprite()->ResizeStretched(followQuest->GetDimension());
	followQuest->SyncSprites();
	followQuest->SetPosition(requiquement->GetPosition({ 0.25f, 1.f }) + Vector2f{ 0.0f, (float)marginOut }, { 0.0f, 0.0f });
	followQuest->GetButtonText()->SetPosition(followQuest->GetPosition());
	followQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	followQuest->GetCollider()->SetIsTrigger(true);
	followQuest->SetOnLeftClick([this] {FollowQuest(m_selectedQuest); });
	//followQuest->GetCollider()->SetShow(true);
	m_slots.push_back(followQuest);

	//Unfollow Quest
	QuestMenuSlot* unfollowQuest = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 160, 69 }, "button", "unfollowQuest", "unfollow Quest", true);
	unfollowQuest->GetSprite()->ResizeStretched(unfollowQuest->GetDimension());
	unfollowQuest->SyncSprites();
	unfollowQuest->SetPosition(requiquement->GetPosition({ 0.75f, 1.0f }) + Vector2f{ 0.0f,(float)marginOut }, { 1.0f, 0.0f });
	unfollowQuest->GetButtonText()->SetPosition(unfollowQuest->GetPosition());
	unfollowQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	unfollowQuest->GetCollider()->SetIsTrigger(true);
	unfollowQuest->SetOnLeftClick([this] {UnfollowQuest(m_selectedQuest); });
	//unfollowQuest->GetCollider()->SetShow(true);
	m_slots.push_back(unfollowQuest);

	//Next Quest
	QuestMenuSlot* nextQuest = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 100, 67 }, "button", "nextQuest", "->", true);
	nextQuest->GetSprite()->ResizeStretched(nextQuest->GetDimension());
	nextQuest->SyncSprites();
	nextQuest->SetPosition({ GetPosition({ 1.0f, 0.5f }).GetX() + -(float)marginOut * 2, unfollowQuest->GetPosition({0.5f, 1.0f}).GetY()}, {1.0f, 1.0f});
	nextQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	nextQuest->GetButtonText()->SetFontSize(42);
	nextQuest->GetButtonText()->SetPosition(nextQuest->GetPosition());
	nextQuest->GetCollider()->SetIsTrigger(true);
	nextQuest->SetOnLeftClick([this] {DisplayPage(GetCurrentPage() + 1); });
	//nextQuest->GetCollider()->SetShow(true);
	m_slots.push_back(nextQuest);

	//Previous Quest
	QuestMenuSlot* previousQuest = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 100, 67 }, "button", "previousQuest", "<-", true);
	previousQuest->GetSprite()->ResizeStretched(previousQuest->GetDimension());
	previousQuest->SyncSprites();
	previousQuest->SetPosition({ requiquement->GetPosition({1.0f, 0.5f}).GetX() + marginOut, nextQuest->GetPosition({0.5f, 0.0f}).GetY() }, { 0.0f, 0.0f });
	previousQuest->GetButtonText()->SetColor(ColorsNamed::Black);
	previousQuest->GetButtonText()->SetFontSize(42);
	previousQuest->GetButtonText()->SetPosition(previousQuest->GetPosition());
	previousQuest->GetCollider()->SetIsTrigger(true);
	previousQuest->SetOnLeftClick([this] {DisplayPage(GetCurrentPage() - 1); });
	//previousQuest->GetCollider()->SetShow(true);
	m_slots.push_back(previousQuest);

	//List of Quest
	QuestMenuSlot* listOfQuest = scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 220, ((int)(requiquement->GetPosition({0.5f, 1.0f}).GetY() - history->GetPosition({0.5f, 0.0f}).GetY())) + 11 }, "buttonHover", "listOfQuest", "", true);
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
	int totalHeight = listOfQuest->GetPosition({0.5f, 0.92f}).GetY() - GetPosition({ 0.5f, 0.08f }).GetY();
	int sizeelement = (int)std::ceil(totalHeight / 6);

	int page = 0;

	for (Quest* quest : *p_questVector)
	{
		QuestMenuSlot* questSlot = m_scene->CreateButton<QuestMenuSlot>({ 0.0f, 0.0f }, { 180, sizeelement }, "button", quest->GetName(), quest->GetName(), true);
		questSlot->GetSprite()->ResizeStretched(questSlot->GetDimension());
		questSlot->SyncSprites();
		questSlot->SetPosition(listOfQuest->GetPosition({0.5f, 0.075f}) + Vector2f{ 0.0f, margin + (questSlot->GetDimension().GetY() * count)}, {0.5f, 0.0f});
		if (QuestManager::Get()->GetActiveQuest() == quest)
		{
			questSlot->GetButtonText()->SetColor(ColorsNamed::Orange);
		}
		else
		{
			questSlot->GetButtonText()->SetColor(ColorsNamed::DarkIndigo);
		}
		questSlot->GetButtonText()->SetFontSize(24);
		questSlot->GetButtonText()->SetPosition(questSlot->GetPosition());
		questSlot->GetCollider()->SetIsTrigger(true);
		questSlot->SetOnLeftClick([this, questSlot] {DisplayQuest(questSlot); });
		//questSlot->GetCollider()->SetShow(true);
		questSlot->SetIsVisible(false);
		questSlot->SetPagesQuest(page);

		m_slots.push_back(questSlot);
		m_quests.push_back(quest);
		count++;
		if (count >= 5)
		{
			count = 0;
			page++;
		}
	}
	m_NBPages = page;
}

void QuestMenu::SetVisible(bool p_visible)
{
	SetIsVisible(p_visible);

	for (QuestMenuSlot* slot : m_slots)
	{
		slot->SetIsVisible(p_visible);
	}
	m_requirementText->SetIsVisible(p_visible);
	m_rewardText->SetIsVisible(p_visible);

}

void QuestMenu::Open(std::vector<Quest*>* p_quest)
{
	m_isOpen = true;
	SetVisible(true);
	SyncQuests(p_quest);
	DisplayPage();

	GetScene<HUDScene>()->HideUi();
}

void QuestMenu::Close()
{
	if (m_isVisible)
		GetScene<HUDScene>()->ShowUi();

	m_isOpen = false;
	SetVisible(false);
	m_selectedQuest = nullptr;
	
	for (QuestMenuSlot* slot : m_slots)
	{
		Text* text = slot->GetButtonText();
		if (slot->GetTextID() == "title")
		{
			text->SetText(" ");
			text->SetPosition(slot->GetPosition());
		}
		else if (slot->GetTextID() == "reward")
		{
			text->SetText(" ");
			text->SetPosition(slot->GetPosition());
		}
		else if (slot->GetTextID() == "history")
		{
			text->SetText(" ");
			text->SetPosition(slot->GetPosition());
		}
		else if (slot->GetTextID() == "requiquement")
		{
			text->SetText(" ");
			text->SetPosition(slot->GetPosition());
		}
		else
		{
			//slot->Destroy();
		}

	}

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
	else
	{
		GetSlot("listOfQuest")->GetButtonText()->SetText(" ");
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
			for (QuestMenuSlot* slot : m_slots)
			{
				Text* text = slot->GetButtonText();
				if (slot->GetTextID() == "title")
				{
					text->SetColor(ColorsNamed::Black);
					text->SetFontSize(42);
					text->SetWrapWidth(slot->GetDimension().GetX() - 20);
					text->HorizontalAlignText(Center);
					text->SetFontStyle(false, true);
					text->SetText(quest->GetName());
					text->SetPosition(slot->GetPosition());
				}
				if (slot->GetTextID() == "reward")
				{
					text->SetColor(ColorsNamed::Black);
					text->SetFontSize(32);
					text->SetWrapWidth(slot->GetDimension().GetX() - 20);
					text->HorizontalAlignText(Center);
					text->SetText(quest->GetRewardDescription());
					text->SetPosition(slot->GetPosition());
				}
				if (slot->GetTextID() == "history")
				{
					text->SetColor(ColorsNamed::Black);
					text->SetFontSize(26);
					text->SetWrapWidth(slot->GetDimension().GetX() - 20);
					text->HorizontalAlignText(Center);
					text->SetText(quest->GetHistoryDescription());
					text->SetPosition(slot->GetPosition());
				}
				if (slot->GetTextID() == "requiquement")
				{
					text->SetColor(ColorsNamed::Black);
					text->SetFontSize(32);
					text->SetWrapWidth(slot->GetDimension().GetX() - 20);
					text->HorizontalAlignText(Center);
					text->SetText(quest->GetRequiquementsDescription());
					text->SetPosition(slot->GetPosition());
				}
			}
			break;
		}
	}
}

void QuestMenu::DisplayPage(int p_page)
{
	int page = Utils::Clamp(p_page, 0, m_NBPages);
	m_currentPage = page;

	for (QuestMenuSlot* slot : m_slots)
	{
		if (slot->GetPagesQuest() == page)
		{
			slot->SetIsVisible(true);
		}
		else if (slot->GetTextID() != "title" && slot->GetTextID() != "reward" && slot->GetTextID() != "history" && slot->GetTextID() != "requiquement" && slot->GetTextID() != "followQuest" && slot->GetTextID() != "unfollowQuest" && slot->GetTextID() != "nextQuest" && slot->GetTextID() != "previousQuest" && slot->GetTextID() != "listOfQuest")
		{
			slot->SetIsVisible(false);
		}
	}
}

void QuestMenu::FollowQuest(Quest* p_quest)
{
	if (p_quest == nullptr)
	{
		return;
	}
	QuestManager::Get()->SetActiveQuest(p_quest->GetName());
	for (QuestMenuSlot* slot : m_slots)
	{
		if (slot->GetTextID() == p_quest->GetName())
		{
			slot->GetButtonText()->SetColor(ColorsNamed::Orange);
		}
	}
}

void QuestMenu::UnfollowQuest(Quest* p_quest)
{
	if (p_quest == nullptr)
	{
		return;
	}
	for (QuestMenuSlot* slot : m_slots)
	{
		if (slot->GetTextID() == p_quest->GetName())
		{
			slot->GetButtonText()->SetColor(ColorsNamed::DarkIndigo);
		}
	}
	QuestManager::Get()->RemoveActiveQuest(p_quest);
}
