#pragma once
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "CollisionManifold.h"

class Entity;

class PhysicsManager
{
private:

	// Liste de toutes les entités enregistrées dans le manager physique
	std::vector<Entity*> m_bodies;

	const float m_PIXELS_PER_METER = 10.f;

	const float m_GRAVITY = 98.1f;

	PhysicsManager() = default;

public:

	static PhysicsManager* Get();

	// Enregistre une entité pour qu'elle soit simulée par le manager
	void RegisterBody(Entity* p_entity);
	// Retire une entité de la simulation
	void UnregisterBody(Entity* p_entity);

	void IntegrateForces(float p_deltaTime);

	void IntegrateVelocities(float p_deltaTime);

	void Repulse(Entity* p_entityA, Entity* p_entityB, const CollisionManifold& p_manifold);

	void ResolveCollision(Entity* p_entityA, Entity* p_entityB, const CollisionManifold& p_manifold);

	float GetPixelsPerMeter() const { return m_PIXELS_PER_METER; }
};