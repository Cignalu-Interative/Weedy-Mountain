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

		m_freezeX = true;
		m_freezeY = true;
		m_freezeRot = true;

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

		m_freezeX = false;
		m_freezeY = false;
		m_freezeRot = true;

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

		m_freezeX = false;
		m_freezeY = false;
		m_freezeRot = false;

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
}

void PhysicsBody::SetFreezeRotation(bool p_freeze)
{
	m_freezeRot = p_freeze;
    if (m_freezeRot)
    {
        m_angularVelocity = 0.0f;
	}
}