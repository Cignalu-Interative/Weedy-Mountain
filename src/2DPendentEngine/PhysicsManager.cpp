#include "pch.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "PhysicsBody.h"
#include "2DPendentLib/Utils.h"
#include <algorithm>

PhysicsManager::PhysicsManager()
{
	m_bodies.reserve(1000); // Réserve de l'espace pour 100 entités au départ
}

PhysicsManager* PhysicsManager::Get()
{
	static PhysicsManager instance;
	return &instance;
}

void PhysicsManager::RegisterBody(Entity* p_entity)
{
	m_bodies.push_back(p_entity);
}

void PhysicsManager::UnregisterBody(Entity* p_entity)
{
	m_bodies.erase(std::remove(m_bodies.begin(), m_bodies.end(), p_entity), m_bodies.end());
}

void PhysicsManager::IntegrateForces(float p_deltaTime)
{
	for (Entity* entity : m_bodies)
	{
		PhysicsBody* body = entity->GetPhysicsBody();

		if (body == nullptr)
			continue;

		if (body->GetBodyType() != ShapeBodyPhysics::Dynamic)
			continue;

		if (body->m_gravityEnabled)
		{
			body->m_force.SetY(body->m_force.GetY() + body->m_mass * m_GRAVITY * body->m_gravityScale);
		}

		// Intégrer la force accumulée -> vélocité linéaire
		// acceleration = force / mass = force * invMass
		body->m_linearVelocity.SetX(body->m_linearVelocity.GetX() + body->m_force.GetX() * body->m_invMass * p_deltaTime);
		body->m_linearVelocity.SetY(body->m_linearVelocity.GetY() + body->m_force.GetY() * body->m_invMass * p_deltaTime);

		// Intégrer le torque accumulé -> vélocité angulaire
		// angularAcceleration = torque / momentOfInertia = torque * invMomentOfInertia
		body->m_angularVelocity += body->m_torque * body->m_invMomentOfInertia * p_deltaTime;

		// Appliquer le damping linéaire et angulaire pour simuler la friction de l'air
		float linearDampingFactor = Utils::LimitMin(1.0f - body->m_linearDamping * p_deltaTime, 0.0f);
		float angularDampingFactor = Utils::LimitMin(1.0f - body->m_angularDamping * p_deltaTime, 0.0f);

		body->m_linearVelocity.SetX(body->m_linearVelocity.GetX() * linearDampingFactor);
		body->m_linearVelocity.SetY(body->m_linearVelocity.GetY() * linearDampingFactor);
		body->m_angularVelocity *= angularDampingFactor;

		// Remettre à zéro les accumulateurs pour la prochaine frame
		body->m_force = { 0.0f, 0.0f };
		body->m_torque = 0.0f;
	}
}

void PhysicsManager::IntegrateVelocities(float p_deltaTime)
{
	for (Entity* entity : m_bodies)
	{
		PhysicsBody* body = entity->GetPhysicsBody();

		if (body == nullptr)
			continue;

		// Static -> ne bouge jamais
		if (body->GetBodyType() == ShapeBodyPhysics::Static)
			continue;

		// Appliquer les contraintes de gel avant de déplacer
		if (body->m_freezeX)
			body->m_linearVelocity.SetX(0.0f);

		if (body->m_freezeY)
			body->m_linearVelocity.SetY(0.0f);

		if (body->m_freezeRot)
			body->m_angularVelocity = 0.0f;

		// Déplacer l'entité via sa propre SetPosition 
		Vector2f currentPosition = entity->GetPosition({ 0.5f, 0.5f });
		entity->SetPosition({
			currentPosition.GetX() + body->m_linearVelocity.GetX() * p_deltaTime,
			currentPosition.GetY() + body->m_linearVelocity.GetY() * p_deltaTime
			});

		// Appliquer la rotation angulaire
		entity->SetAngle(entity->GetAngle() + body->m_angularVelocity * p_deltaTime);
	}
}

void PhysicsManager::Repulse(Entity* p_entityA, Entity* p_entityB, const CollisionManifold& p_manifold)
{
	if (p_manifold.penetration <= 0.f)
		return;

	PhysicsBody* bodyA = p_entityA->GetPhysicsBody();
	PhysicsBody* bodyB = p_entityB->GetPhysicsBody();

	float invMassA = (bodyA != nullptr) ? bodyA->m_invMass : 1.f;
	float invMassB = (bodyB != nullptr) ? bodyB->m_invMass : 1.f;

	float totalInvMass = invMassA + invMassB;

	if (totalInvMass == 0.f)
		return; 

	// Correction positionnelle proportionnelle aux masses inverses
	// (Baumgarte-lite : on ne corrige pas 100 % pour éviter les vibrations)
	const float correctionPercent = 0.8f;
	float correctionMagnitude = (p_manifold.penetration / totalInvMass) * correctionPercent;

	Vector2f correction = {
		p_manifold.normal.GetX() * correctionMagnitude,
		p_manifold.normal.GetY() * correctionMagnitude
	};

	// Déplacer A dans le sens de la normale 
	if (bodyA != nullptr && bodyA->GetBodyType() != ShapeBodyPhysics::Static)
	{
		Vector2f posA = p_entityA->GetPosition({ 0.5f, 0.5f });

		p_entityA->SetPosition({
			posA.GetX() - correction.GetX() * invMassA,
			posA.GetY() - correction.GetY() * invMassA
			});
	}

	// Déplacer B dans le sens opposé 
	if (bodyB != nullptr && bodyB->GetBodyType() != ShapeBodyPhysics::Static)
	{
		Vector2f posB = p_entityB->GetPosition({ 0.5f, 0.5f });
		p_entityB->SetPosition({
			posB.GetX() + correction.GetX() * invMassB,
			posB.GetY() + correction.GetY() * invMassB
			});
	}

	// Mettre à jour isGrounded si la collision est principalement verticale
	if (p_manifold.normal.GetY() > 0.5f)
		p_entityA->SetIsGrounded(true);

	if (p_manifold.normal.GetY() < -0.5f)
		p_entityB->SetIsGrounded(true);
}

void PhysicsManager::ResolveCollision(Entity* p_entityA, Entity* p_entityB, const CollisionManifold& p_manifold)
{
	PhysicsBody* bodyA = p_entityA->GetPhysicsBody();
	PhysicsBody* bodyB = p_entityB->GetPhysicsBody();

	float invMassA = (bodyA != nullptr) ? bodyA->m_invMass : 0.f;
	float invMassB = (bodyB != nullptr) ? bodyB->m_invMass : 0.f;

	float totalInvMass = invMassA + invMassB;

	if (totalInvMass == 0.f)
		return; 

	Vector2f velA = (bodyA != nullptr) ? bodyA->m_linearVelocity : Vector2f{ 0.f, 0.f };
	Vector2f velB = (bodyB != nullptr) ? bodyB->m_linearVelocity : Vector2f{ 0.f, 0.f };

	// Vélocité relative le long de la normale de collision
	Vector2f relativeVelocity = { velB.GetX() - velA.GetX(), velB.GetY() - velA.GetY() };
	float velocityAlongNormal = relativeVelocity.GetX() * p_manifold.normal.GetX() + relativeVelocity.GetY() * p_manifold.normal.GetY();

	// Les corps s'éloignent déjà l'un de l'autre -> pas besoin de résoudre
	if (velocityAlongNormal > 0.f)
		return;

	// Coefficient de restitution
	float restitutionA = (bodyA != nullptr) ? bodyA->m_restitution : 0.f;
	float restitutionB = (bodyB != nullptr) ? bodyB->m_restitution : 0.f;
	float restitution = Utils::LimitMin(restitutionA, restitutionB);

	// Magnitude de l'impulsion scalaire
	float impulseMagnitude = -(1.f + restitution) * velocityAlongNormal / totalInvMass;

	Vector2f impulse = {
		p_manifold.normal.GetX() * impulseMagnitude,
		p_manifold.normal.GetY() * impulseMagnitude
	};

	// Appliquer l'impulsion aux bodies
	if (bodyA != nullptr && bodyA->GetBodyType() == ShapeBodyPhysics::Dynamic)
	{
		bodyA->m_linearVelocity.SetX(bodyA->m_linearVelocity.GetX() - impulse.GetX() * invMassA);
		bodyA->m_linearVelocity.SetY(bodyA->m_linearVelocity.GetY() - impulse.GetY() * invMassA);
	}

	if (bodyB != nullptr && bodyB->GetBodyType() == ShapeBodyPhysics::Dynamic)
	{
		bodyB->m_linearVelocity.SetX(bodyB->m_linearVelocity.GetX() + impulse.GetX() * invMassB);
		bodyB->m_linearVelocity.SetY(bodyB->m_linearVelocity.GetY() + impulse.GetY() * invMassB);
	}

	// Nettoyer les résidus de vélocité le long de la normale 
	if (bodyA != nullptr && bodyA->GetBodyType() == ShapeBodyPhysics::Dynamic)
	{
		float velAlongNormalA = bodyA->m_linearVelocity.GetX() * p_manifold.normal.GetX() + bodyA->m_linearVelocity.GetY() * p_manifold.normal.GetY();

		if (Utils::AbsValue(velAlongNormalA) < bodyA->m_seuilToBounce)
		{
			bodyA->m_linearVelocity.SetX(bodyA->m_linearVelocity.GetX() - velAlongNormalA * p_manifold.normal.GetX());
			bodyA->m_linearVelocity.SetY(bodyA->m_linearVelocity.GetY() - velAlongNormalA * p_manifold.normal.GetY());
		}
	}

	if (bodyB != nullptr && bodyB->GetBodyType() == ShapeBodyPhysics::Dynamic)
	{
		float velAlongNormalB = bodyB->m_linearVelocity.GetX() * p_manifold.normal.GetX() + bodyB->m_linearVelocity.GetY() * p_manifold.normal.GetY();

		if (Utils::AbsValue(velAlongNormalB) < bodyB->m_seuilToBounce)
		{
			bodyB->m_linearVelocity.SetX(bodyB->m_linearVelocity.GetX() - velAlongNormalB * p_manifold.normal.GetX());
			bodyB->m_linearVelocity.SetY(bodyB->m_linearVelocity.GetY() - velAlongNormalB * p_manifold.normal.GetY());
		}
	}

	// Friction tangentielle lors d'une collision principalement verticale
	if (Utils::AbsValue(p_manifold.normal.GetY()) > 0.5f)
	{
		float frictionA = (bodyA != nullptr) ? bodyA->m_friction : 0.f;
		float frictionB = (bodyB != nullptr) ? bodyB->m_friction : 0.f;
		float friction = Utils::LimitMin(frictionA, frictionB);

		if (bodyA != nullptr && bodyA->GetBodyType() == ShapeBodyPhysics::Dynamic)
		{
			float velX = bodyA->m_linearVelocity.GetX() * (1.0f - friction);
			bodyA->m_linearVelocity.SetX(Utils::AbsValue(velX) < 5.0f ? 0.0f : velX);
		}

		if (bodyB != nullptr && bodyB->GetBodyType() == ShapeBodyPhysics::Dynamic)
		{
			float velX = bodyB->m_linearVelocity.GetX() * (1.0f - friction);
			bodyB->m_linearVelocity.SetX(Utils::AbsValue(velX) < 5.0f ? 0.0f : velX);
		}
	}
}