#pragma once
#include "2DPendentLib/Vector2.h"
#include "ShapeBodyPhysics.h"
#include "2DPendentLib/Transformable.h"

class PhysicsBody : public Transformable
{
private:

	ShapeBodyPhysics m_shapeBodyPhysics;

    // Mass & inertie \\

    // Résistance aux forces linéaires. Plus c'est lourd, moins une force le déplace.
    float m_mass = 1.0f;

    // 1 / m_mass, précalculé pour éviter les divisions à chaque frame.
    // Si == 0.0f -> le body est immovable (Static / Kinematic).
    float m_invMass = 1.0f;

    // Résistance à la rotation. Dépend de la masse ET de la forme du collider.
    // Une boîte large résiste plus à tourner qu'un petit cercle.
    float m_momentOfInertia = 1.0f;

    // 1 / m_momentOfInertia, même raison que m_invMass.
    float m_invMomentOfInertia = 1.0f;

	// Cinematique Linéaire \\

    // Vitesse actuelle en pixels/seconde. Mise à jour par IntegrateForces,
    // puis utilisée par IntegrateVelocity pour déplacer le body.
    Vector2f m_linearVelocity = { 0.0f, 0.0f };

    // Accumulateur de forces pour la frame courante.
    // Remis à zéro après IntegrateForces.
    Vector2f m_force = { 0.0f, 0.0f };

    // Friction de l'air sur le déplacement. Ralentit progressivement
    // la velocity linéaire même sans collision.
    float m_linearDamping = 0.1f;

	// Cinematique Angulaire \\

    // Vitesse de rotation actuelle en radians/seconde.
    float m_angularVelocity = 0.0f;

    // Accumulateur de couple (rotation) pour la frame courante.
    // Remis à zéro après IntegrateForces.
    float m_torque = 0.0f;

    // Friction de l'air sur la rotation. Évite qu'un objet
    // tourne infiniment après une collision.
    float m_angularDamping = 0.1f;

    // Gravité \\

    // Active ou non la gravité sur ce body.
    // Static et Kinematic ont ça à false.
    bool  m_gravityEnabled = true;

    // Multiplicateur de la gravité mondiale.
    // 0.5f = gravité lunaire, 2.0f = gravité renforcée.
    float m_gravityScale = 2.5f;

    // Vitesse maximale de chute pour éviter les problèmes de tunneling à haute vitesse.
	//float m_maxFallSpeed = 1000.0f;

	// Matériau physique \\

    // Coefficient de rebond lors d'une collision.
    // 0.0f = aucun rebond (argile), 1.0f = rebond parfait (balle de ping-pong).
    float m_restitution = 0.3f;

    // Résistance au glissement contre une autre surface.
    // 0.0f = glace, 1.0f = caoutchouc.
    float m_friction = 0.5f;

	// Contraintes de mouvement \\

    // Empêche tout déplacement sur l'axe X.
    // Utile pour un personnage qui ne doit pas être poussé latéralement.
    bool m_freezePositionX = false;

    // Empêche tout déplacement sur l'axe Y.
    bool m_freezePositionY = false;

    // Empêche toute rotation physique.
    // Presque toujours true pour les personnages jouables.
    bool m_freezeRotation = false;
  
    // Seuil de vitesse en dessous duquel un objet s'arrête de rebondir pour éviter les rebonds infinis à basse vitesse.
	float m_seuilToBounce = 20.0f;

    /*Vector2f m_velocity = { 0.0f, 0.0f };

    Vector2f m_accumulatedForces = { 0.0f, 0.0f };

    float m_gravity = 0;

    float m_maxFallSpeed = 0;

    float m_mass = 1.0f;

    float m_friction = 20;

    bool m_isGrounded = false;

    bool m_gravityEnabled = true;*/

    /*void ApplyGravity(float p_deltaTime);

    void ApplyFriction(float p_deltaTime);

    void ClampVelocity();*/

public:

    PhysicsBody(ShapeBodyPhysics p_shapeBodyPhysics);

	void SetBodyType(ShapeBodyPhysics p_shapeBodyPhysics) { m_shapeBodyPhysics = p_shapeBodyPhysics; }
	ShapeBodyPhysics GetBodyType() const { return m_shapeBodyPhysics; }

    // Met à jour m_mass et m_invMass en même temps pour éviter les erreurs de division.
	// Un mass de 0.0f signifie que le body est immovable (Static / Kinematic).
	// Par exemple, si vous appelez SetMass(2.0f), m_mass sera mis à 2.0f et m_invMass sera mis à 0.5f.
	void SetMass(float p_mass);
	float GetMass() const { return m_mass; }
	float GetInvMass() const { return m_invMass; }

    // Un moment d'inertie de 0.0f signifie que le body est infiniement résistant à la rotation.
	// Par exemple, si vous appelez SetMomentOfInertia(0.0f), m_momentOfInertia sera mis à 0.0f et m_invMomentOfInertia sera mis à 0.0f,
    // ce qui signifie que le body ne tournera jamais.
	// Un moment d'inertie de 1.0f signifie que le body a une résistance à la rotation normale.
	void SetMomentOfInertia(float p_momentOfInertia);
	float GetMomentOfInertia() const { return m_momentOfInertia; }

    // Applique une force linéaire au centre de masse du body. Par exemple, si vous appelez ApplyForce({ 10.0f, 0.0f }),
    // le body recevra une force de 10 unités vers la droite.
	// Cette force sera accumulée dans m_force et utilisée pour calculer l'accélération du body lors de la mise à jour.
	// Si le body est Static ou Kinematic, cette fonction n'aura aucun effet, car ces types de corps ne sont pas affectés par les forces.
	void ApplyForce(const Vector2f& p_force);
	Vector2f GetForce() const { return m_force; }

    // Applique un couple (force de rotation) au body. Par exemple, si vous appelez ApplyTorque(5.0f),
	// le body recevra un couple de 5 unités dans le sens des aiguilles d'une montre.
	// Cette force de rotation sera accumulée dans m_torque et utilisée pour calculer l'accélération angulaire du body lors de la mise à jour.
	// Si le body est Static ou Kinematic, cette fonction n'aura aucun effet, car ces types de corps ne sont pas affectés par les forces de rotation.
	// Un couple positif fait tourner le body dans le sens des aiguilles d'une montre, tandis qu'un couple négatif le fait tourner dans le sens inverse.
	void ApplyTorque(float p_torque);
	float GetTorque() const { return m_torque; }

	// Applique une impulsion instantanée au body à un point de contact spécifique. Par exemple, si vous appelez ApplyImpulse({ 0.0f, -10.0f }, { 1.0f, 0.0f }),
	// le body recevra une impulsion de 10 unités vers le haut au point de contact situé à 1 unité à droite du centre de masse.
	// Cette impulsion changera instantanément la vitesse linéaire et angulaire du body en fonction de sa masse, de son moment d'inertie, et de la position du point de contact.
	// Si le body est Static ou Kinematic, cette fonction n'aura aucun effet, car ces types de corps ne sont pas affectés par les impulsions.
	// L'impulsion linéaire change la vitesse linéaire du corps en fonction de sa masse.
	void ApplyImpulse(const Vector2f& p_impulse, const Vector2f& p_contactPoint);
    Vector2f GetLinearVelocity() const { return m_linearVelocity; }
	float GetAngularVelocity() const { return m_angularVelocity; }

	// Résout une collision entre deux bodies en appliquant les forces d'impulsion nécessaires pour les séparer et les faire rebondir.
	// Cette fonction prend en paramètres les deux bodies impliqués dans la collision, le point de contact, la normale de collision, et la pénétration (profondeur de chevauchement).
	// La Normale de collision est un vecteur unitaire qui pointe de bodyA vers bodyB
	static void ResolveCollisionPhysics(PhysicsBody* p_bodyA, PhysicsBody* p_bodyB, const Vector2f& p_contactPoint, const Vector2f& p_normal);

	// Intègre les forces accumulées pour calculer la nouvelle vélocité du body.
    // Cette fonction est appelée à chaque frame pour mettre à jour la vélocité en fonction des forces appliquées.
	// Si la gravité est activée pour ce body, elle sera appliquée en ajoutant une force vers le bas proportionnelle à la masse du body et à l'échelle de gravité.
	// Ensuite, la force accumulée sera utilisée pour calculer l'accélération linéaire, puis la vélocité linéaire sera mise à jour en fonction de cette accélération.
	// De même, le couple accumulé sera utilisé pour calculer l'accélération angulaire, puis la vélocité angulaire sera mise à jour en fonction de cette accélération.
	// Enfin, le damping linéaire et angulaire sera appliqué pour simuler la friction de l'air,
    // en réduisant progressivement les vélocités linéaire et angulaire au fil du temps.
	// Si le body est Static ou Kinematic, cette fonction n'aura aucun effet, car ces types de corps ne sont pas affectés par les forces.
	void IntegrateForces(float p_deltaTime);

    // Intègre la vélocité pour calculer la nouvelle position du body.
	// Cette fonction est appelée à chaque frame après IntegrateForces pour déplacer le body en fonction de sa vélocité.
	// La vélocité linéaire est utilisée pour calculer le déplacement du body en fonction du temps écoulé (deltaTime).
	// La vélocité angulaire est utilisée pour calculer la rotation du body en fonction du temps écoulé.
	// Si le body est Static ou Kinematic, cette fonction n'aura aucun effet, car ces types de corps ne sont pas déplacés par la vélocité.
	void IntegrateVelocity(float p_deltaTime);

    // Permet de définir la vélocité linéaire du body. Par exemple, si vous appelez SetLinearVelocity({ 5.0f, 0.0f }),
	// le body aura une vélocité de 5 unités vers la droite.
	// Par Exemple pour sauter car la vélocité sera mise à jour instantanément, contrairement à ApplyForce qui applique une force sur la durée.
	void SetLinearVelocity(const Vector2f& p_velocity) { m_linearVelocity = p_velocity; }

    // Permet de geler ou dégeler la rotation du body. Par exemple, si vous appelez SetFreezeRotation(true),
	// le body ne tournera plus du tout, même s'il reçoit des forces de rotation ou des impulsions.
	// Cela est particulièrement utile pour les personnages jouables, qui ne doivent pas tourner physiquement, mais peuvent toujours être affectés par les forces linéaires.
	void SetFreezeRotation(bool p_freeze);

   /* const Vector2f& GetVelocity() const { return m_velocity; }

    void SetVelocity(const Vector2f& p_vel) { m_velocity = p_vel; }

    void AddVelocity(const Vector2f& p_vel) { m_velocity = m_velocity + p_vel; }

    void SetVelocityX(float p_x) { m_velocity.SetX(p_x); }
    void SetVelocityY(float p_y) { m_velocity.SetY(p_y); }

    bool IsGrounded() const { return m_isGrounded; }
    void SetGrounded(bool p_grounded) { m_isGrounded = p_grounded; }

    float GetGravity() const { return m_gravity; }
    void  SetGravity(float p_gravity) { m_gravity = p_gravity; }

    bool  IsGravityEnabled() const { return m_gravityEnabled; }
    void  SetGravityEnabled(bool p_enabled) { m_gravityEnabled = p_enabled; }

    float GetMass() const { return m_mass; }
    void  SetMass(float p_mass) { m_mass = p_mass; }

    float GetFriction() const { return m_friction; }
    void  SetFriction(float p_friction) { m_friction = p_friction; }

    void ApplyForce(const Vector2f& p_force);

    void ApplyImpulse(const Vector2f& p_impulse);

    void ResetVelocity() { m_velocity = { 0.0f, 0.0f }; }

    bool IsFalling() const { return m_velocity.GetY() > 0.0f; }
    bool IsRising()  const { return m_velocity.GetY() < 0.0f; } */
};