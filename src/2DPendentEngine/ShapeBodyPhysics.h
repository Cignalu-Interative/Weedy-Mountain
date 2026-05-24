#pragma once
#include <cstdint>

enum class ShapeBodyPhysics : uint8_t
{
	// Un corps statique ne bouge pas et n'est pas affecté par les forces,
	// il est utilisé pour les objets immobiles comme le sol ou les murs.
	Static,
	// Un corps cinématique peut être déplacé manuellement par le code,
	// mais n'est pas affecté par les forces, il est utilisé pour les objets qui doivent se déplacer de manière contrôlée, comme les plateformes mobiles.
	Kinematic,
	// Un corps dynamique est affecté par les forces et peut bouger librement,
	// il est utilisé pour les objets qui doivent réagir aux collisions et aux forces, comme les personnages ou les projectiles.
	Dynamic,

};
