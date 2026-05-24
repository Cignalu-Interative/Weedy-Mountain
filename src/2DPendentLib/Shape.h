#pragma once

//Forme d'un collider
enum class Shape : uint8_t
{
	//Collider de type cercle
	Circle,
	//Collider de type rectangle
	Rectangle,

	//Nombre de formes de collider
	count
};