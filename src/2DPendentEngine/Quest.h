#pragma once
#include <string>
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "StateMachine.h"
#include "Entity.h"
#include "2DPendentLib/Text.h"
#include "2DPendentLib/Rectangle.h"
#include "2DPendentLib/Circle.h"
#include "2DPendentLib/Sprite.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentLib/Drawable.h"

struct QuestRequirement
{
	int s_requiredAmount = 0;
	int s_harvestedAmount = 0;

	Sprite* s_sprite = nullptr;

	void SetRequiredAmount(int p_amount) { s_requiredAmount = p_amount; }
	void AddHarvestedAmount(int p_amount) { s_harvestedAmount += p_amount; }

	int GetRequiredAmount() const { return s_requiredAmount; }
	int GetHarvestedAmount() const { return s_harvestedAmount; }
};

struct QuestReward
{
	std::string s_itemID; // id pas trouvé dans la base de donnée alors thune
	int s_amount = 0;

	std::string GetItemID() const { return s_itemID; }

	void SetAmount(int p_amount) { s_amount = p_amount; }

	int GetAmount() const { return s_amount; }
};

class Quest : public Drawable
{
private:

	std::string m_name;
	Entity* m_questOwner = nullptr;

	bool m_isNotStarted = true;
	bool m_isInProcess = false;
	bool m_isFinished = false;

	// signifie que le joueur perdra l'objet que la quête demande à la fin de la quête
	bool m_lostItem = false;

	std::unordered_map<size_t, QuestRequirement> m_requirements;

	Circle* m_circleQuest = nullptr;
	Text* m_textQuest = nullptr;

public:

	Quest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner);
	~Quest();

	void SetName(const std::string& p_name) { m_name = p_name; }
	const std::string& GetName() const { return m_name; }

	void SetQuestOwner(Entity* p_owner) { m_questOwner = p_owner; }
	Entity* GetQuestOwner() const { return m_questOwner; }

	void SetNotStarted() { m_isNotStarted = true; m_isInProcess = false; m_isFinished = false; }
	void SetInProcess() { m_isNotStarted = false; m_isInProcess = true; m_isFinished = false; }
	void SetFinished() { m_isNotStarted = false; m_isInProcess = false; m_isFinished = true; }

	void AddRequirement(const std::string& p_idTexture, int p_requiredAmount);
	QuestRequirement GetRequirement(size_t p_emplacement) const;

	size_t GetRequirementsCount() const { return m_requirements.size(); }

	Circle* GetQuestCircle() const { return m_circleQuest; }
	Text* GetQuestText() const { return m_textQuest; }

	void UpdateQuest();

	void SetTextQuest();

	void ResizeGrid();

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};