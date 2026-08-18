#pragma once
#include "Entity.h"
#include "Savable.h"

class FurnaceUi;
class Furnace : public Entity, public Savable
{
private :

	FurnaceUi* m_furnaceUi = nullptr;
	
	bool m_isColliding = false;

	std::string m_saveTag;
	static uint32_t s_furnaceCount;
	uint32_t m_myID = 0;

	void OnInitialize() override;
	void OnDestroy() override;
	void OnEvent() override;
	void OnUpdate() override;

	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) override;
public:
	void SetSaveTag(const std::string& p_tag) { m_saveTag = p_tag; }

	std::string GetSaveID() const override
	{
		return m_saveTag.empty() ? ("furnace_" + std::to_string(m_myID)) : m_saveTag;
	}
	void Save(nlohmann::json& p_out) override;
	void Load(const nlohmann::json& p_in) override;
};