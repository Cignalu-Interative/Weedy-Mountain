#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
//Version 1.0.1

//Composition de la couleur
struct Color
{
	//Rouge
	Uint8 r;
	//Vert
	Uint8 g;
	//Bleu
	Uint8 b;
	//Alpha (transparence)
	Uint8 a;
};

//Couleurs nommées
struct ColorsNamed
{
	// === RED ===
	static constexpr Color LightRed = { 255, 120, 120, 255 };
	static constexpr Color Red = { 220, 40, 40, 255 };
	static constexpr Color DarkRed = { 139, 0, 0, 255 };

	// === ORANGE ===
	static constexpr Color LightOrange = { 255, 170, 90, 255 };
	static constexpr Color Orange = { 230, 120, 30, 255 };
	static constexpr Color DarkOrange = { 204, 85, 0, 255 };

	// === YELLOW ===
	static constexpr Color LightYellow = { 255, 235, 140, 255 };
	static constexpr Color Yellow = { 255, 200, 60, 255 };
	static constexpr Color DarkYellow = { 184, 134, 11, 255 };

	// === GREEN ===
	static constexpr Color LightGreen = { 120, 220, 140, 255 };
	static constexpr Color Green = { 40, 170, 80, 255 };
	static constexpr Color DarkGreen = { 0, 100, 0, 255 };

	// === BLUE ===
	static constexpr Color LightBlue = { 120, 190, 255, 255 };
	static constexpr Color Blue = { 40, 110, 220, 255 };
	static constexpr Color DarkBlue = { 0, 51, 102, 255 };

	// === INDIGO ===
	static constexpr Color LightIndigo = { 140, 130, 255, 255 };
	static constexpr Color Indigo = { 80, 60, 180, 255 };
	static constexpr Color DarkIndigo = { 46, 8, 84, 255 };

	// === VIOLET ===
	static constexpr Color LightViolet = { 210, 150, 255, 255 };
	static constexpr Color Violet = { 160, 80, 220, 255 };
	static constexpr Color DarkViolet = { 90, 0, 140, 255 };

	// === PINK ===
	static constexpr Color LightPink = { 255, 160, 200, 255 };
	static constexpr Color Pink = { 240, 80, 150, 255 };
	static constexpr Color DarkPink = { 180, 30, 100, 255 };

	// === BROWN ===
	static constexpr Color LightBrown = { 200, 150, 100, 255 };
	static constexpr Color Brown = { 150, 95, 60, 255 };
	static constexpr Color DarkBrown = { 90, 55, 35, 255 };

	// === BLACK / GRAY / WHITE ===
	static constexpr Color DarkGray = { 90, 90, 90, 255 };
	static constexpr Color Black = { 0, 0, 0, 255 };
	static constexpr Color White = { 255, 255, 255, 255 };
	static constexpr Color LightGray = { 200, 200, 200, 255 };
	static constexpr Color VeryLightGray = { 230, 230, 230, 255 };
	static constexpr Color Gray = { 130, 130, 130, 255 };
	static constexpr Color DimGray = { 110, 110, 110, 255 };

	// === SILVER / GOLD ===
	static constexpr Color VeryLightSilver = { 245, 245, 245, 255 };
	static constexpr Color Silver = { 190, 190, 190, 255 };
	static constexpr Color DarkSilver = { 140, 140, 140, 255 };

	static constexpr Color LightGold = { 255, 230, 150, 255 };
	static constexpr Color Gold = { 255, 210, 80, 255 };
	static constexpr Color DarkGold = { 184, 134, 11, 255 };

	// === TURQUOISE ===
	static constexpr Color LightTurquoise = { 140, 240, 235, 255 };
	static constexpr Color Turquoise = { 60, 200, 190, 255 };
	static constexpr Color DarkTurquoise = { 0, 140, 140, 255 };
};
