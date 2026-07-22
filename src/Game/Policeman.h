#pragma once
#include "PNJ.h"
#include "StateMachine.h"
#include "Pathfinder.h"

enum class PolicemanState
{
	IDLE,
	PATROL,
	CHASE,

	COUNT
};

class Policeman : public PNJ
{
private:
	
	bool m_isPlayerDetected = false;
	bool m_isLoosingPlayer = false;

	bool m_isIdleCooldownEnded = false;

	bool m_isCollidingWithPlayer = false;

	StateMachine<Policeman> m_stateMachine;
	
	std::vector<Tile*> m_path;
	int m_pathIndex;

	Pathfinder m_pathfinder;

	Vector2f m_playerPos;

	int m_demiConeSize = 40;
	float m_limitedCos = std::cos(m_demiConeSize * (Utils::PI / 180));

protected:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;
	void OnCollisionStay(Collider* p_self, Collider* p_other, CollisionSide p_side) override;
	void OnCollisionExit(Collider* p_self, Collider* p_other, CollisionSide p_side) override;

public:

	void SetupStateMachine();

	void SetIdleCooldownEnded(bool p_value) { m_isIdleCooldownEnded = p_value; }

	bool IsPlayerDetected() { return m_isPlayerDetected; }
	bool IsIdleCooldownEnded() { return m_isIdleCooldownEnded; }
	bool IsLoosingPlayer() { return m_isLoosingPlayer; }
	bool IsCollidingWithPlayer() { return m_isCollidingWithPlayer; }
	bool IsArrived() { return m_pathIndex >= m_path.size(); }

	void StartChase();
	void StartPatrol();

	bool IsInVisionCone(Vector2f p_goalPos);

	int GetPathIndex() { return m_pathIndex; }
	std::vector<Tile*> GetPath() { return m_path; }
	Vector2f GetPlayerPos() { return m_playerPos; }

	void SetPathIndex(int p_newPathIndex) { m_pathIndex = p_newPathIndex; }
	void SetPath(std::vector<Tile*> p_newPath) { m_path = p_newPath; }

};

