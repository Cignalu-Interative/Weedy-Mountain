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
	bool m_isUnreachable = false;

	bool m_isIdleCooldownEnded = false;

	bool m_isCollidingWithPlayer = false;

	Vector2f m_facingDir = { 0.f, 0.f };

	StateMachine<Policeman> m_stateMachine;
	
	std::vector<NavTile*> m_path;
	int m_pathIndex = 0;

	Pathfinder m_pathfinder;

	Vector2f m_playerPos;

	int m_demiConeSize = 55;
	float m_limitedCos = std::cos(m_demiConeSize * (Utils::PI / 180));

protected:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnEvent() override;
	void OnCollisionStay(Collider* p_self, Collider* p_other, CollisionSide p_side) override;
	void OnCollisionExit(Collider* p_self, Collider* p_other, CollisionSide p_side) override;



public:
	void OnDestroy() override {};
	void SetupStateMachine();

	void SetIdleCooldownEnded(bool p_value) { m_isIdleCooldownEnded = p_value; }

	bool IsPlayerDetected() { return m_isPlayerDetected; }
	bool IsIdleCooldownEnded() { return m_isIdleCooldownEnded; }
	bool IsLoosingPlayer() { return m_isLoosingPlayer; }
	bool IsCollidingWithPlayer() { return m_isCollidingWithPlayer; }
	bool IsArrived() { return m_pathIndex >= m_path.size() - 1; }

	bool IsUnreachable() { return m_isUnreachable; }

	void SetIsLoosingPlayer(bool p_newValue) { m_isLoosingPlayer = p_newValue; }
	void SetIsUnreachable(bool p_newValue) { m_isUnreachable = p_newValue; }
	void SetIsPlayerDetected(bool p_newValue) { m_isPlayerDetected = p_newValue; }

	void StartChase();
	void StartPatrol();

	bool IsInVisionCone(Vector2f p_goalPos);

	int GetPathIndex() { return m_pathIndex; }
	std::vector<NavTile*> GetPath() { return m_path; }
	Vector2f GetPlayerPos() { return m_playerPos; }

	void SetPathIndex(int p_newPathIndex) { m_pathIndex = p_newPathIndex; }
	void SetPath(std::vector<NavTile*> p_newPath) { m_path = p_newPath; }

	void SetState(PolicemanState p_state);

	void SetFacingDir(Vector2f p_newValue) { m_facingDir = p_newValue; }
};

