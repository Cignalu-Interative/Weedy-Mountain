#include "pch.h"
#include "NotificationManager.h"
#include "ItemData.h"
#include "ItemDataBase.h"
#include "SpriteContainer.h"


NotificationManager::~NotificationManager()
{
	if (m_notif == nullptr)
		return;

	if (m_notif != nullptr && m_notif->ToDestroy() == false)
	{
		m_notif->Destroy();
	}
}

void NotificationManager::Initialize(Scene* scene)
{
	m_scene = scene;
}

void NotificationManager::AddNotification(const std::string& background, const std::string& icon, const std::string& title, const std::string& subtitle, const std::string& quantity)
{
	NotificationData data;

	data.background = background;
	data.icon = icon;
	data.title = title;
	data.subtitle = subtitle;
	data.quantity = quantity;

	m_notifications.push(data);

	if (m_notif == nullptr)
		ShowNextNotification();
}

void NotificationManager::AddNotification(const NotificationData& data)
{
	AddNotification(data.background, data.icon, data.title, data.subtitle, data.quantity);
}

void NotificationManager::Update()
{
	if (m_notif == nullptr)
	{
		ShowNextNotification();
		return;
	}

	if (m_notif->ToDestroy())
	{
		m_notif = nullptr;
		/*ShowNextNotification()*/ //detruite a la fin de la frame donc on return pour pas risquer de récupérer l'autre texte ? (sinon on va recréer une notif alors qu les anciens textes sont pas deete ?)
		return; 
	}
}

void NotificationManager::ShowNextNotification()
{
	if (m_scene == nullptr || m_notif != nullptr || m_notifications.empty() == true)
		return;

	NotificationData data = m_notifications.front();
	m_notifications.pop();

	m_notif = m_scene->CreateEntity<Notification>({ 208, 58 }, { 396, 96 }, data.background, Shape::Rectangle, true);

	m_notif->Initialize(data);
}
















































Notification::~Notification()
{
	if (m_title)
		m_scene->RemoveGeometry("title");

	if (m_subtitle)
		m_scene->RemoveGeometry("subtitle");

	if (m_quantity)
		m_scene->RemoveGeometry("quantity");
}

void Notification::Initialize(const NotificationData& data)
{
	SetDrawLayer(300);

	SetSprite(data.background);//background

	//icon
	AddSprite("icon", data.icon, { 54, 54 }, 1, { -150.0f, 0.0f }, true);
		
	//trxt titer
	m_scene->CreateText("title", { 27 * 3, 10 * 3 }, data.title, { 255, 255, 255, 255 }, true, false);

	//subtitle texte
	m_scene->CreateText("subtitle", { 27 * 3, 20 * 3 }, CheckAndCutSubtitel(data.subtitle), { 255, 255, 255, 255 }, true, false);

	//quantity texte
	m_scene->CreateText("quantity", { GetPosition({1.0f, 0.0f}).GetX() - 48, GetPosition().GetY() + 0 }, data.quantity, { 255, 255, 255, 255 }, true, false);

	m_title = m_scene->GetGeometry<Text>("title");
	m_subtitle = m_scene->GetGeometry<Text>("subtitle");
	m_quantity = m_scene->GetGeometry<Text>("quantity");

	m_title->SetPosition({ 27 * 3 + 10, 10 * 3 + 10 }, { 0.0f, 0.5f });
	m_subtitle->SetPosition({ 27 * 3 + 10, 20 * 3 + 10 }, { 0.0f, 0.5f });
}

void Notification::OnUpdate()
{
	m_lifeTime -= m_scene->GetDeltaTime();

	if (m_lifeTime <= 0)
	{
		Destroy();
		return;
	}
}




std::string Notification::CheckAndCutSubtitel(const std::string& subtitle)
{
	if (subtitle.size() <= 20)
		return subtitle;

	std::string result;

	for (int i = 0; i < 20; i++)
	{
		result += subtitle[i];
	}

	return result + "...";
}