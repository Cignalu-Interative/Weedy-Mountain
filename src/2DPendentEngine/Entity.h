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
class SpriteContainer;
class Animation;

class Entity : public Transformable
{
	void SetToDestroy(bool p_toDestroy) { m_toDestroy = p_toDestroy; }

protected:

	std::vector<SpriteContainer*> m_sprites; //Sprites de l'entité
	std::vector<Collider*> m_colliders; //Colliders de l'entité
	PhysicsBody* m_physicsBody = nullptr; //Composant physique de l'entité

	Scene* m_scene = nullptr; //Pointeur vers la scène à laquelle l'entité appartient

	Animation* m_animation = nullptr;

	Vector2f m_direction = { 0.0f, 0.0f };

	float m_speed = 100;

	const float m_PIXELS_PER_METER = 10; // Constante pour convertir les pixels en mètres pour la physique

	int m_tag = 0; //Tag de l'entité, utilisé pour identifier le type de l'entité (ex: 0 = joueur, 1 = ennemi, 2 = plateforme, etc...)
	int m_drawLayer = 1;

	bool m_toDestroy = false; //Bool pour savoir si l'entité doit être détruite
	bool m_rigidBody = false; //Bool pour savoir si les corps sont rigide et dont ne peuvent pas se traverser
	bool m_clampToWindow = false; //Bool pour savoir si l'entité doit être clamp à la fenêtre
	bool m_resetDirection = true; //Bool pour savoir si la direction doit être réinitialisée à chaque frame, utile pour les entités contrôlées par le joueur

	bool m_isGrounded = false; //Bool pour savoir si l'entité est au sol, utile pour les entités contrôlées par le joueur

	bool m_isVisible = true; //Bool pour savoir si l'entité doit être dessinée à l'écran

	bool m_isStatic = false;
	//Fonction pour initialiser l'entité, notamment en définissant sa position, sa dimension, son sprite et son collider
	void Initialize(Vector2f p_position, Vector2i p_dimention, const std::string& p_path, Render* f_render, Shape p_shape, bool p_isGUI = false, bool p_animation = false, const std::string& p_jsonAnimation = "");
	//Fonction pour mettre à jour l'entité, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void Update();
	//Fonction pour dessiner l'entité à l'écran
	void Draw(Render* f_render, Window* f_window);

	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique d'initialisation
	virtual void OnInitialize() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de gestion des événements
	virtual void OnEvent() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de mise à jour
	virtual void OnUpdate() {};
	//Fonction virtuelle pour permettre aux classes qui héritent de Entity de définir leur propre logique de collision
	virtual void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) {}
	virtual void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide _side) {}
	virtual void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide _side) {}
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

	//Fonction pour marquer l'entité pour destruction, elle sera détruite à la fin de la frame en cours
	void Destroy();
	//Obtenir le bool pour savoir si l'entité doit être détruite
	bool ToDestroy() const { return m_toDestroy; }

	void SetDrawLayer(int p_drawLayer) { m_drawLayer = p_drawLayer; }
	int GetDrawLayer() const { return m_drawLayer; }

	//Obtenir la forme du premier collider de l'entité
	Shape GetShape() const { return static_cast<Shape>(GetCollider()->GetShape()); }
	//Obtenir la forme d'un collider de l'entité avec son ID
	Shape GetShape(const std::string& p_id) const { return static_cast<Shape>(GetCollider(p_id)->GetShape()); }

	//Obtenir le delta time en milliseconds depuis la dernière frame, en utilisant la fonction GetDeltaTime() de la scène à laquelle l'entité appartient
	float GetDeltaTime() const;

	//Set le pointeur de la scene à laquelle l'entité appartient
	void SetScene(Scene* p_scene) { m_scene = p_scene; }

	//Ajouter un collider à l'entité
	Collider* AddCollider(const std::string& p_id, Vector2i p_dimension, Shape p_shape, Vector2f p_offset = { 0.f,0.f }, bool p_isTrigger = false, uint32_t p_layer = 1, uint32_t p_mask = 0xFFFFFFFF);

	//Obtenir le collider de l'entité avec son ID
	Collider* GetCollider(const std::string& p_id) const;
	//Obtenir le premier collider de l'entité
	Collider* GetCollider() const;
	//Obtenir tous les colliders de l'entité
	const std::vector<Collider*>& GetColliders() const { return m_colliders; }
	//Supprimer le collider de l'entité avec son ID
	void RemoveCollider(const std::string& p_id);
	// Sync tous les colliders de l'entity
	void SyncColliders();

	//Ajouter un sprite à l'entité
	SpriteContainer* AddSprite(const std::string& p_id, const std::string& p_texturePath, Vector2i p_dimension, int p_zOrder = 0, Vector2f p_offset = { 0.f,0.f }, bool p_isGUI = false);
	//Changer le premier sprite de l'entité (id "default")
	void SetSprite(const std::string& p_texturePath);
	//Changer le sprite de l'entité avec son ID
	void SetSprite(const std::string& p_id, const std::string& p_texturePath);
	//Supprimer le sprite de l'entité avec son ID
	void RemoveSprite(const std::string& p_id);

	SpriteContainer* GetSpriteContainer(const std::string& p_id) const;
	SpriteContainer* GetSpriteContainer() const; // retourne le "default"
	Sprite* GetSprite(const std::string& p_id) const;
	Sprite* GetSprite() const; // retourne le sprite "default"

	void SetSpriteVisible(const std::string& p_id, bool p_visible);
	void SetAllSpritesVisible(bool p_visible);

	void SyncSprites();

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

	//modifie la visibilité de tout les collider de l'entité
	void ShowCollider(bool p_show);            
	//modifie la visibilité du collider de l'entité avec son ID
	void ShowCollider(const std::string& p_id, bool p_show); 

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

	void SetAnimationByName(const std::string& p_nameAnimation);

	void SetIsStatic(bool p_isStatic) { m_isStatic = p_isStatic; }
	bool GetIsStatic() { return m_isStatic; }
	//Liste des class amie avec la class Entity
	friend class CollisionManager;
	friend class Application;
	friend class Scene;
};

#include "Entity.inl"