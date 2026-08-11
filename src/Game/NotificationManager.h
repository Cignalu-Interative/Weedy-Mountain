#pragma once

#include "button.h"
#include "Entity.h"

#include <queue>
#include <string>


class Scene;
class Text;
class Notification;


struct NotificationData
{
	std::string icon;
	std::string title;
	std::string subtitle;
	std::string quantity;
	std::string background;
};


class NotificationManager
{
private:

	Scene* m_scene = nullptr;

	Notification* m_notif = nullptr;

	std::queue<NotificationData> m_notifications;


public:

	NotificationManager() = default;
	~NotificationManager();

	void Initialize(Scene* scene);

	void AddNotification(const std::string& background, const std::string& icon, const std::string& title, const std::string& subtitle, const std::string& quantity);
	void AddNotification(const NotificationData& data);

	void Update();


private:

	void ShowNextNotification();
};










class Notification : public Entity
{

private:

	Text* m_title = nullptr;
	Text* m_subtitle = nullptr;
	Text* m_quantity = nullptr;

	float m_lifeTime = 3.0f;

public:

	Notification() = default;
	~Notification();

	void Initialize(const NotificationData& data);

	void OnUpdate() override;

private:

	std::string CheckAndCutSubtitel(const std::string& subtitle);


};