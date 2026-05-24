#pragma once

// Enum pour représenter les différentes orientations du son par rapport au joueur, en degrés
//NORTH est définie à 0 degrés, NORTHEAST est définie à 45 degrés, EAST est définie à 90 degrés,
//SOUTHEAST est définie à 135 degrés, SOUTH est définie à 180 degrés, SOUTHWEST est définie à 225 degrés,
//WEST est définie à 270 degrés, NORTHWEST est définie à 315 degrés
enum class SoundOrientation : uint8_t
{
	//North est définie à 0 degrés
	NORTH = 0,
	//Northeast est définie à 45 degrés
	NORTHEAST = 45,
	//East est définie à 90 degrés
	EAST = 90,
	//Southeast est définie à 135 degrés
	SOUTHEAST = 135,
	//South est définie à 180 degrés
	SOUTH = 180,
	//Southwest est définie à 225 degrés
	SOUTHWEST = 225,
	//West est définie à 270 degrés
	WEST = 270,
	//Northwest est définie à 315 degrés
	NORTHWEST = 315
};
