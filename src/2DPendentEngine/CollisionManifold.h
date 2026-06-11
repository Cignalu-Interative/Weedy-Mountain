#pragma once
#include "2DPendentLib/Vector2.h"
#include "Collider.h"

// Structure contenant toutes les informations d'une collision entre deux colliders.
// Produite par CollisionManager::ComputeManifold() et consommée par PhysicsManager.
struct CollisionManifold
{
	// Normale de collision pointant de B vers A (unitaire)
	Vector2f normal = { 0.f, 0.f };

	// Profondeur de pénétration entre les deux colliders
	float penetration = 0.f;

	// Point de contact approximatif (centre entre les deux colliders)
	Vector2f contactPoint = { 0.f, 0.f };

	// Face touchée du point de vue de A et de B
	CollisionSide sideA = CollisionSide::NONE;
	CollisionSide sideB = CollisionSide::NONE;

	// Vrai si la collision a été effectivement détectée
	bool valid = false;
};