#pragma once
//Version 1.0.1

//Définition du format Audio
enum AudioFormat : uint8_t
{
	//Format de fichier audio non reconnue ou non initialisée
	NONE,
	// Format de fichier audio WAV
	WAV,
	// Format de fichier audio MP3
	MP3,

	// Nombre total de formats audio pris en charge
	COUNT
};