#pragma once
#include "2DPendentLib/Rectangle.h"
#include "2DPendentLib/Circle.h"
#include "2DPendentEngine/Collider.h"

class Entity;
class Collision
{
private:

	//Constructeur de Collision
	Collision() = default;
	//Destructeur de Collision
	~Collision() = default;	

	//Cette fonction permet de projeter les coins d'un rectangle sur un axe donné,
	// elle retourne une paire de float représentant les valeurs minimale et maximale de la projection des coins du rectangle sur l'axe donné
	// Exemple d'utilisation : si vous avez un rectangle avec des coins aux positions [(100, 100), (150, 100), (150, 150), (100, 150)]
	// et un axe de projection de (1, 0) (c'est-à-dire l'axe des x),
	// la fonction Project retournerait une paire de float contenant les valeurs minimale et maximale de la projection des coins du rectangle sur l'axe des x : (100, 150).
	static std::pair<float, float> Project(const std::array<Vector2f, 4>& p_corners, Vector2f p_axis);

	//Collision entre deux rectangles
	static bool Collision_AABB(const Collider& p_rectA, const Collider& p_rectB);
	//Collision entre deux cercles
	static bool Collision_CBC(const Collider& p_circleA, const Collider& p_circleB);
	//Collision entre un rectangle et un cercle
	static bool Collision_ARC(const Collider& p_rect, const Collider& p_circle);

	//Fonctions de répulsion pour séparer les entités en collision pour des collider de type Rectangle et Rectangle
	static void Repulse_AABB(Entity* p_entityRectA, Entity* p_entityRectB);
	//Fonctions de répulsion pour séparer les entités en collision pour des collider de type Cercle et Cercle
	static void Repulse_CBC(Entity* p_entityCircleA, Entity* p_entityCircleB);
	//Fonctions de répulsion pour séparer les entités en collision pour des collider de type Rectangle et Cercle
	static void Repulse_ARC(Entity* p_entityRect, Entity* p_entityCircle);

public:

	friend class RectangleCollider;

	//Vérifie si deux colliders sont en collision
	static bool IsColliding(const Collider& p_colliderA, const Collider& p_colliderB);

	static void Repulse(Entity* p_entityA, Entity* p_entityB);




	// En public pour l'instant rapide pour Rectangle collider
	//Collision entre deux OBB (Oriented Bounding Box)
	//La fonction n'est pas vérifier pour l'instant
	//Ne pas utiliser pour l'instant il faut dabord implémenter la rotation pour les colliders et les sprites
	static bool Collision_OBB(const Collider& p_rectA, const Collider& p_rectB);

};