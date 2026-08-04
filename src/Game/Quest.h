#pragma once
#include <string>
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "2DPendentEngine/Entity.h"
#include "2DPendentLib/Text.h"
#include "2DPendentLib/Rectangle.h"
#include "2DPendentLib/Circle.h"
#include "2DPendentLib/Sprite.h"
#include "2DPendentLib/AssetManager.h"
#include "2DPendentLib/Drawable.h"
#include "ItemData.h"
#include "ItemDataBase.h"

enum class CheckQuest
{
	UNDEFINED,
	AddItem,

	Max

};

struct QuestRequirement
{
	CheckQuest s_checkType = CheckQuest::UNDEFINED;

	ItemData* s_itemData = nullptr;

	int s_requiredAmount = 0;
	int s_harvestedAmount = 0;

	Text* s_requiredText = nullptr;
	Text* s_harvestedText = nullptr;

	bool s_isCompleted = false;

	void SetCheckType(CheckQuest p_checkType) { s_checkType = p_checkType; }
	CheckQuest GetCheckType() const { return s_checkType; }

	void SetRequiredAmount(int p_amount) { s_requiredAmount = p_amount; }
	void AddHarvestedAmount(int p_amount)
	{
		s_harvestedAmount += p_amount;
		if (s_harvestedAmount >= s_requiredAmount)
		{
			s_isCompleted = true;
			s_harvestedAmount = s_requiredAmount;
		}
	}

	int GetRequiredAmount() const { return s_requiredAmount; }
	int GetHarvestedAmount() const { return s_harvestedAmount; }

	void SetItemData(ItemData* p_itemData) { s_itemData = p_itemData; }
	ItemData* GetItemData() const { return s_itemData; }

	bool IsCompleted() const { return s_isCompleted; }

	void SetVisible(bool p_visible) { s_requiredText->SetIsVisible(p_visible); s_harvestedText->SetIsVisible(p_visible); }
};

struct QuestReward
{
	ItemData* s_itemData = nullptr;
	int s_amount = 0;

	Text* s_amountText = nullptr;

	void SetItemData(ItemData* p_itemData) { s_itemData = p_itemData; }
	ItemData* GetItemData() const { return s_itemData; }

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

	std::unordered_map<size_t, QuestRequirement*> m_requirements;
	std::unordered_map<size_t, QuestReward*> m_rewards;

	Entity* m_zone = nullptr;

	Vector2i m_dimSpriteRequirement = { 32, 32 };

	Scene* m_scene = nullptr;

	Entity* m_icone = nullptr;

	bool m_QuestIsCompleted = false;

	bool m_rewardclaimed = false;

	std::string m_historyDescription;

	Vector2i m_refDimension = {}; // dimention de reference donné a la creation de la quest pour le redimensionnement de la grille d'affichage des requirements

public:

	Quest(const std::string& p_name, Vector2f p_position, Vector2i p_dimension, Entity* p_owner, Scene* p_scene);
	~Quest();

	void SetName(const std::string& p_name) { m_name = p_name; }
	const std::string& GetName() const { return m_name; }

	void SetQuestOwner(Entity* p_owner) { m_questOwner = p_owner; }
	Entity* GetQuestOwner() const { return m_questOwner; }

	void SetNotStarted() { m_isNotStarted = true; m_isInProcess = false; m_isFinished = false; }
	void SetInProcess() { m_isNotStarted = false; m_isInProcess = true; m_isFinished = false; }
	void SetFinished() { m_isNotStarted = false; m_isInProcess = false; m_isFinished = true; }

	void AddRequirement(CheckQuest p_CheckType, const std::string& p_idTexture, int p_requiredAmount);
	QuestRequirement* GetRequirement(size_t p_emplacement) const;
	size_t GetRequirementsCount() const { return m_requirements.size(); }

	void AddReward(ItemData* p_itemData, int p_amount);
	QuestReward* GetReward(size_t p_emplacement) const;
	size_t GetRewardsCount() const { return m_rewards.size(); }

	void UpdateQuest();

	void SetTextQuest();

	void ResizeGrid();

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

	void SetScene(Scene* p_scene) { m_scene = p_scene; }
	Scene* GetScene() const { return m_scene; }

	bool IsQuestCompleted() const { return m_QuestIsCompleted; }

	void SetHistoryDescription(const std::string& p_description) { m_historyDescription = p_description; }

	std::string GetRewardDescription();

	std::string GetHistoryDescription();

	std::string GetRequiquementsDescription();

	void SetVisible(bool p_visible);

	bool GetVisible() const { return (m_zone->GetIsVisible() || m_icone->GetIsVisible()); }

	Entity* GetZone() const { return m_zone; }

	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });
	Vector2f GetPosition(Vector2f p_anchor = { 0.5f, 0.5f }) const;

	// La dimention de la quest doit etre un multiple de 2 genre 32 64 ou 128 sinon c'est pas beau (la grille d'affichage sera bugger)
	void SetRefDimension(Vector2i p_dimension) { m_refDimension = p_dimension; }
	Vector2i GetRefDimension() const { return m_refDimension; }
};