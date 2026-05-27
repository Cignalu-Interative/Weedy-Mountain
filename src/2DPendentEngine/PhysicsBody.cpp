#include "pch.h"
#include "PhysicsBody.h"
#include "Utils.h"

PhysicsBody::PhysicsBody(ShapeBodyPhysics p_shapeBodyPhysics)
{
	m_shapeBodyPhysics = p_shapeBodyPhysics;

	/*std::cout << "IntegrateForces, adresse body=" << this
		<< " type=" << (int)m_shapeBodyPhysics << std::endl;*/

    switch (p_shapeBodyPhysics)
    {
    case ShapeBodyPhysics::Static:

        m_gravityEnabled = false;

		m_mass = 0.0f;
		m_invMass = 0.0f;

		m_momentOfInertia = 0.0f;
		m_invMomentOfInertia = 0.0f;

		m_linearDamping = 0.0f;
		m_angularDamping = 0.0f;

        m_restitution = 0.15f;
		m_friction = 0.3f;

		m_freezePositionX = true;
		m_freezePositionY = true;
		m_freezeRotation = true;

		m_seuilToBounce = 20.0f;
        break;

    case ShapeBodyPhysics::Kinematic:

        m_gravityEnabled = false;

		m_mass = 0.0f;
		m_invMass = 0.0f;

		m_momentOfInertia = 0.0f;
		m_invMomentOfInertia = 0.0f;

		m_linearDamping = 0.0f;
		m_angularDamping = 0.0f;

		m_restitution = 0.0f;
		m_friction = 0.0f;

		m_freezePositionX = false;
		m_freezePositionY = false;
		m_freezeRotation = true;

		m_seuilToBounce = 20.0f;
        break;

    case ShapeBodyPhysics::Dynamic:

		m_gravityEnabled = true;

		m_mass = 1.0f;
		m_invMass = 1.0f;

		m_momentOfInertia = 1.0f;
		m_invMomentOfInertia = 1.0f;

		m_linearDamping = 0.1f;
		m_angularDamping = 0.1f;

		m_restitution = 0.3f;
		m_friction = 0.5f;

		m_freezePositionX = false;
		m_freezePositionY = false;
		m_freezeRotation = false;

		m_seuilToBounce = 20.0f;
        break;
    }
}

void PhysicsBody::SetMass(float p_mass)
{
	m_mass = Utils::LimitMin(p_mass, 0.0f);
	m_invMass = (m_mass > 0.0f) ? (1.0f / m_mass) : 0.0f;
}

void PhysicsBody::SetMomentOfInertia(float p_momentOfInertia)
{
    m_momentOfInertia = Utils::LimitMin(p_momentOfInertia, 0.0f);
	m_invMomentOfInertia = (m_momentOfInertia > 0.0f) ? (1.0f / m_momentOfInertia) : 0.0f;
}

void PhysicsBody::ApplyForce(const Vector2f& p_force)
{
    if (m_shapeBodyPhysics != ShapeBodyPhysics::Dynamic)
    {
        return;
	}
	m_force += p_force;
}

void PhysicsBody::ApplyTorque(float p_torque)
{
    if (m_shapeBodyPhysics != ShapeBodyPhysics::Dynamic)
    {
        return;
    }
	m_torque += p_torque;
}

void PhysicsBody::ApplyImpulse(const Vector2f& p_impulse, const Vector2f& p_contactPoint)
{
    if (m_shapeBodyPhysics != ShapeBodyPhysics::Dynamic)
    {
        return;
    }
	// L'impulsion linéaire change la vitesse linéaire du corps en fonction de sa masse.
	// La formule pour calculer la nouvelle vitesse linéaire après l'application d'une impulsion est la suivante : v' = v + (impulse / mass),
    // où v est la vitesse linéaire actuelle du corps, impulse est l'impulsion appliquée, et mass est la masse du corps.
	// Ce qui sert à calculer la nouvelle vitesse linéaire du corps après l'application de l'impulsion.
    m_linearVelocity.SetX(m_linearVelocity.GetX() + m_invMass * p_impulse.GetX());
    m_linearVelocity.SetY(m_linearVelocity.GetY() + m_invMass * p_impulse.GetY());

	m_angularVelocity += m_invMomentOfInertia * Utils::Cross(p_contactPoint, p_impulse);
}

void PhysicsBody::ResolveCollisionPhysics(PhysicsBody* p_bodyA, PhysicsBody* p_bodyB, const Vector2f& p_contactPoint, const Vector2f& p_normal)
{
	float totalInvMass = p_bodyA->m_invMass + p_bodyB->m_invMass;
	if (totalInvMass == 0.0f)
	{
		return; // deux Static -> rien à faire
	}

	// Calculer la vélocité relative entre les deux corps
	Vector2f relativeVelocity = p_bodyB->GetLinearVelocity() - p_bodyA->GetLinearVelocity();

	// Calculer la vélocité le long de la normale de collision
	float velocityAlongNormal = relativeVelocity.GetX() * p_normal.GetX() + relativeVelocity.GetY() * p_normal.GetY();

	if (velocityAlongNormal > 0) // Les corps s'éloignent l'un de l'autre, pas besoin de résoudre la collision
	{
	    return;
	}

	// Calculer le coefficient de restitution (rebond) en prenant le minimum des deux corps
    float restitution = Utils::LimitMax(p_bodyA->m_restitution, p_bodyB->m_restitution);

	// Calculer l'impulsion de collision
    float impulseMagnitude = -(1 + restitution) * velocityAlongNormal / totalInvMass;

	// Appliquer l'impulsion aux deux corps
    Vector2f impulse = impulseMagnitude * p_normal;

	// Appliquer l'impulsion à bodyA dans la direction opposée
	// Appliquer l'impulsion à bodyB dans la direction normale
    p_bodyA->ApplyImpulse(-impulse, p_contactPoint);
    p_bodyB->ApplyImpulse(impulse, p_contactPoint);

	// Nettoyer le résidu de vélocité le long de la normale pour bodyA
	float velA = p_bodyA->GetLinearVelocity().GetX() * p_normal.GetX()
		+ p_bodyA->GetLinearVelocity().GetY() * p_normal.GetY();
	if (Utils::AbsValue(velA) < p_bodyA->m_seuilToBounce)
	{
		p_bodyA->m_linearVelocity.SetX(p_bodyA->m_linearVelocity.GetX() - velA * p_normal.GetX());
		p_bodyA->m_linearVelocity.SetY(p_bodyA->m_linearVelocity.GetY() - velA * p_normal.GetY());
	}

	// Nettoyer le résidu de vélocité le long de la normale pour bodyB
	float velB = p_bodyB->GetLinearVelocity().GetX() * p_normal.GetX()
		+ p_bodyB->GetLinearVelocity().GetY() * p_normal.GetY();
	if (Utils::AbsValue(velB) < p_bodyB->m_seuilToBounce)
	{
		p_bodyB->m_linearVelocity.SetX(p_bodyB->m_linearVelocity.GetX() - velB * p_normal.GetX());
		p_bodyB->m_linearVelocity.SetY(p_bodyB->m_linearVelocity.GetY() - velB * p_normal.GetY());
	}

	if (Utils::AbsValue(p_normal.GetY()) > 0.5f) // Collision principalement verticale
	{
		float friction = Utils::LimitMax(p_bodyA->m_friction, p_bodyB->m_friction);

		float velXA = p_bodyA->m_linearVelocity.GetX();
		p_bodyA->m_linearVelocity.SetX(velXA * (1.0f - friction));

		float velXB = p_bodyB->m_linearVelocity.GetX();
		p_bodyB->m_linearVelocity.SetX(velXB * (1.0f - friction));

		if (Utils::AbsValue(p_bodyA->m_linearVelocity.GetX()) < 5.0f)
			p_bodyA->m_linearVelocity.SetX(0.0f);
	}
}

void PhysicsBody::IntegrateForces(float p_deltaTime)
{
	//std::cout << "IntegrateForces called, type=" << (int)m_shapeBodyPhysics << std::endl;


    if (m_shapeBodyPhysics != ShapeBodyPhysics::Dynamic)
    {
        return;
	}
	/*std::cout << "gravityEnabled=" << m_gravityEnabled
		<< " mass=" << m_mass
		<< " invMass=" << m_invMass << std::endl;*/

	if (m_gravityEnabled) // Appliquer la gravité en ajoutant une force vers le bas proportionnelle à la masse du corps et à l'échelle de gravité.
    {
		m_force.SetY(m_force.GetY() + m_mass * 981.0f * m_gravityScale);
	}

	// Appliquer la force accumulée pour calculer l'accélération linéaire, puis mettre à jour la vélocité linéaire en fonction de cette accélération.
	// La formule pour calculer l'accélération linéaire à partir de la force est la suivante : acceleration = force / mass.
	m_linearVelocity.SetX(m_linearVelocity.GetX() + m_force.GetX() * m_invMass * p_deltaTime);
	m_linearVelocity.SetY(m_linearVelocity.GetY() + m_force.GetY() * m_invMass * p_deltaTime);

	// Appliquer le couple accumulé pour calculer l'accélération angulaire, puis mettre à jour la vélocité angulaire en fonction de cette accélération.
	// La formule pour calculer l'accélération angulaire à partir du couple est la suivante : angularAcceleration = torque / momentOfInertia.
	m_angularVelocity += m_torque * m_invMomentOfInertia * p_deltaTime;

	// Appliquer le damping linéaire et angulaire pour simuler la friction de l'air, en réduisant progressivement les vélocités linéaire et angulaire au fil du temps.
	float linearDampingFactor = Utils::LimitMin(1.0f - m_linearDamping * p_deltaTime, 0.0f);
	float angularDampingFactor = Utils::LimitMin(1.0f - m_angularDamping * p_deltaTime, 0.0f);

	// Appliquer le facteur de damping à la vélocité linéaire et angulaire pour réduire progressivement leur valeur au fil du temps.
	m_linearVelocity.SetX(m_linearVelocity.GetX() * linearDampingFactor);
	m_linearVelocity.SetY(m_linearVelocity.GetY() * linearDampingFactor);
	m_angularVelocity *= angularDampingFactor;

	// Remettre à zéro les accumulateurs de forces et de couple pour la prochaine frame, afin que les forces appliquées soient recalculées à chaque frame.
	m_force = { 0.0f, 0.0f };
	m_torque = 0.0f;
}

void PhysicsBody::IntegrateVelocity(float p_deltaTime)
{
    // Kinematic -> se déplace via m_linearVelocity assigné par script
    // Dynamic -> m_linearVelocity a été calculé par IntegrateForces puis corrigé par les impulses de collision
    // Les deux passent par ici
    if (m_shapeBodyPhysics == ShapeBodyPhysics::Static)
    {
        return;
    }

	if (m_freezePositionX) // Empêche tout déplacement sur l'axe X.
    {
        m_linearVelocity.SetX(0.0f);
	}
	if (m_freezePositionY) // Empêche tout déplacement sur l'axe Y.
    {
		m_linearVelocity.SetY(0.0f);
    }
	if (m_freezeRotation) // Empêche toute rotation physique.
    {
        m_angularVelocity = 0.0f;
	}

	//Vector2f currentPosition = GetPosition();
	Move(Vector2f(m_linearVelocity.GetX() * p_deltaTime, m_linearVelocity.GetY() * p_deltaTime));
	//SetPosition(currentPosition);

	SetAngle(GetAngle() + m_angularVelocity * p_deltaTime);

	/*std::cout << "IntegrateVelocity, nouvelle position="
		<< GetPosition().GetX() << " " << GetPosition().GetY() << std::endl;*/
}

void PhysicsBody::SetFreezeRotation(bool p_freeze)
{
	m_freezeRotation = p_freeze;
    if (m_freezeRotation)
    {
        m_angularVelocity = 0.0f;
	}
}









//void PhysicsBody::ApplyForce(const Vector2f& p_force)
//{
//    m_accumulatedForces = m_accumulatedForces + p_force;
//}
//
//void PhysicsBody::ApplyImpulse(const Vector2f& p_impulse)
//{
//    m_velocity = m_velocity + p_impulse;
//}
//
//void PhysicsBody::ApplyGravity(float p_deltaTime)
//{
//    if (!m_gravityEnabled || m_isGrounded)
//    {
//        return;
//    }
//
//    m_velocity.SetY(m_velocity.GetY() + m_gravity * p_deltaTime);
//}
//
//void PhysicsBody::ApplyFriction(float p_deltaTime)
//{
//    if (!m_isGrounded)
//    {
//        return;
//    }
//
//    float frictionFactor = 1.0f - m_friction * p_deltaTime;
//    frictionFactor = Utils::LimitMin(frictionFactor, 0.0f);
//
//    m_velocity.SetX(m_velocity.GetX() * frictionFactor);
//
//    if (Utils::AbsValue(m_velocity.GetX()) < 0.5f)
//    {
//        m_velocity.SetX(0.0f);
//    }
//}
//
//void PhysicsBody::ClampVelocity()
//{
//	m_velocity.SetY(Utils::Clamp(m_velocity.GetY(), -m_maxFallSpeed, m_maxFallSpeed));
//}