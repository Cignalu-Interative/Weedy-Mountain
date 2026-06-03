#pragma once
#include <string>
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Sprite.h"
#include "2DPendentLib/Transformable.h"
#include "Collider.h"
#include "2DPendentLib/Shape.h"
#include "PhysicsBody.h"
#include "ShapeBodyPhysics.h"

class Render;
class Window;
class Scene;
class Entity : public Transformable
{
protected:

	Sprite* m_sprite = nullptr; //Sprite de l'entité
	Collider* m_collider = nullptr; //Collider de l'entité
	PhysicsBody* m_physicsBody = nullptr; //Composant physique de l'entité

	Scene* m_scene = nullptr; //Pointeur vers la scène à laquelle l'entité appartient

	Vector2f m_direction = { 0.0f, 0.0f };

	float m_speed = 100;

	const float m_PIXELS_PER_METER = 10; // Constante pour convertir les pixels en mètres pour la physique

	int m_tag = 0; //Tag de l'entité, utilisé pour identifier le type de l'entité (ex: 0 = joueur, 1 = ennemi, 2 = plateforme, etc...)

	bool m_toDestroy = false; //Bool pour savoir si l'entité doit être détruitey
	bool m_rigidBody = false; //Bool pour savoir si les corps sont rigide et dont ne peuvent pas se traverser
	bool m_clampToWindow = false; //Bool pour savoir si l'entité doit être clamp à la fenêtre
	bool m_resetDirection = true; //Bool pour savoir si la direction doit être réinitialisée à chaque frame, utile pour les entités contrôlées par le joueur

	bool m_isGrounded = false; //Bool pour savoir si l'entité est au sol, utile pour les entités contrôlées par le joueur

	bool m_isVisible = true; //Bool pour savoir si l'entité doit être dessinée à l'écran

	//Fonction pour initialiser l'entité, notamment en définissant sa position, sa dimension, son sprite et son collider
	void Initialize(Vector2f p_position, Vector2i p_dimention, const std::string& p_path, Render* f_render, Shape p_shape, bool p_isGUI = false);
	//Fonction pour mettre à jour l'entité, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void Update();
	//Fonction pour dessiner l'entité à l'écran
	void Draw(Render* f_render, Window* f_window);
	//Fonction pour marquer l'entité pour destruction, elle sera détruite à la fin de la frame en cours
	void Destroy();

	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique d'initialisation
	virtual void OnInitialize() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de gestion des événements
	virtual void OnEvent() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de mise à jour
	virtual void OnUpdate() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de collision
	virtual void OnCollision(Entity* other) {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de collision avec les bords de la fenêtre
	virtual void OnOutBorder() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de destruction
	virtual void OnDestroy() {};

	//Fonction pour mettre à jour la position de l'entité en fonction des forces appliquées sur son corps physique
	//Cette fonction est appelée à chaque frame pour intégrer les forces et mettre à jour la position de l'entité en fonction de sa vélocité.
	//Elle utilise les fonctions IntegrateForces et IntegrateVelocity de la classe PhysicsBody
	// pour calculer la nouvelle position de l'entité en fonction des forces appliquées et de sa vélocité actuelle.
	void UpdateMovement_Forces(float p_deltaTime);
	//Fonction pour mettre à jour la position de l'entité en fonction de sa vélocité
	//Cette fonction est appelée à chaque frame pour mettre à jour la position de l'entité en fonction de sa vélocité actuelle.
	//Elle utilise la fonction SetPosition de la classe Transformable pour déplacer l'entité en fonction de sa vélocité et du temps écoulé (deltaTime).
	void UpdateMovement_Velocity(float p_deltaTime);

public:

	//Constructeur par défaut de Entity
	Entity() = default;
	//Destructeur de Entity, il est virtuel pour permettre aux classes qui héritent de Entity de définir leur propre logique de destruction
	virtual ~Entity();

	template<typename T>
	T* GetScene() const;

	//Obtenir le pointeur vers la scène à laquelle l'entité appartient
	Scene* GetScene() const { return m_scene; }

	template<typename T>
	T* CreateEntity(Vector2f p_position, Vector2i p_dimension, const std::string& p_path, Shape p_shape);

	void InitialisePhysicsBody(ShapeBodyPhysics p_shapeBodyPhysics);

	PhysicsBody* GetPhysicsBody() { return m_physicsBody; }

	//Obtenir le bool pour savoir si l'entité doit être détruite
	bool ToDestroy() const { return m_toDestroy; }
	void SetToDestroy(bool p_toDestroy) { m_toDestroy = p_toDestroy; }

	//Obtenir la forme du collider de l'entité
	Shape GetShape() const { return static_cast<Shape>(m_collider->GetShape()); }

	//Obtenir le delta time en milliseconds depuis la dernière frame, en utilisant la fonction GetDeltaTime() de la scène à laquelle l'entité appartient
	float GetDeltaTime() const;

	//Set le pointeur de la scene à laquelle l'entité appartient
	void SetScene(Scene* p_scene) { m_scene = p_scene; }

	template<typename T>
	T* GetCollider() const;

	//Obtenir le collider de l'entité
	Collider* GetCollider() { return m_collider; }
	//Obtenir le sprite de l'entité
	Sprite* GetSprite() { return m_sprite; }

	//Définir si l'entité est un corps rigide
	void SetRigidBody(bool p_rigidBody) { m_rigidBody = p_rigidBody; }
	//Obtenir le bool pour savoir si l'entité est un corps rigide
	bool IsRigidBody() const { return m_rigidBody; }

	// Définir la direction de l'entity
	void SetDirection(Vector2f p_direction) { m_direction = p_direction; }
	// Connaitre la direction de l'entity
	Vector2f GetDirection() { return m_direction; }

	// Définir la speed de l'entity
	void SetSpeed(float p_speed) { m_speed = p_speed; }
	// Connaitre la speed de l'entity
	float GetSpeed() { return m_speed; }

	//bool IsCollidingWith(Entity* p_other);

	// SetPosition de entity (set la position de entity son collider et son sprite)
	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5f, 0.5f });

	// SetDimension de entity (set la taille de entity son collider et son sprite)
	void SetDimension(Vector2i p_dimemsion);

	void SetAngle(float p_angle);

	void SetClampToWindow(bool p_clamp) { m_clampToWindow = p_clamp; }
	bool GetClampToWindow() const { return m_clampToWindow; }

	void ShowCollider(bool p_show) { m_collider->SetShow(p_show); }

	void SetIsGUI(bool p_isGUI);
	bool GetIsGUI() const;

	void SetTag(int p_tag) { m_tag = p_tag; }
	int GetTag() const { return m_tag; }

	void SetResetDirection(bool p_reset) { m_resetDirection = p_reset; }
	bool GetResetDirection() const { return m_resetDirection; }

	// Move pour entity move aussi le collider et le sprite
	void Move(Vector2f p_translation);

	void SetIsGrounded(bool p_isGrounded) { m_isGrounded = p_isGrounded; }
	bool GetIsGrounded() const { return m_isGrounded; }

	void SetIsVisible(bool p_isVisible);

	bool GetIsVisible() const { return m_isVisible; };

	//Liste des class amie avec la class Entity
	friend class Application;
	friend class Scene;
};

#include "Entity.inl"