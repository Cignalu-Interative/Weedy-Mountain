#pragma once
#include <string>
#include "2DPendentLib/Vector2.h"


class Sprite;
class ParserSpriteSheet;
struct InfoAnimation;
class Animation
{
	Sprite* m_sprite = nullptr;

	bool m_hasAnimation = true;

	int m_currentFrame = 0;

	float m_frameTime; // duree de l'animation en secondes
	float m_currentTimer = 0;

	std::string m_nameAnimation;

	bool m_isFacingRight = true;
	bool m_loop = true;

	int m_currentRow = 0; // pour les animations en ligne (ex: idle, walk, etc.)

	// Truc pour auto-detecter la taille d'une frame dans un sprite sheet
	Vector2i m_frameSize{ 16,16 };
	Vector2i m_framesNB {10, 10};
	int m_totalFrames = 0;

	ParserSpriteSheet* m_parser = nullptr;
	//const InfoAnimation* m_infoAnimation = nullptr;

public:
	Animation() = default;
	~Animation() = default;

	//Initialise l'animation en associant un sprite, en définissant la durée de l'animation, le nom de l'animation et si elle doit boucler ou non
	void Init(Sprite* p_sprite, const std::string& p_nameAnimation = "default", bool p_loop = true, const std::string& p_infoAnimationSpriteSheet = "");

	//Met à jour l'animation en fonction du temps écoulé depuis la dernière frame, en avançant la frame courante si nécessaire et en gérant le bouclage de l'animation
	void Update(float p_deltaTime);

	//Permet de régler automatiquement la taille d'une frame en fonction de la dimension du sprite et du nombre de frames en X et Y
	void AutoDetectFrameSize(Vector2i p_spriteDimention);

	//Set le bool pour savoir si le sprite fait face à droite ou à gauche (pour le flip horizontal)
	void SetIsFacingRight(bool p_facingRight) {m_isFacingRight = p_facingRight;}
	
	//Obtenir le bool pour savoir si le sprite fait face à droite ou à gauche (pour le flip horizontal)
	bool IsFacingRight() const { return m_isFacingRight; }

	//Set la durée de l'animation en secondes
	void SetDuration(float p_duration) { m_frameTime = p_duration; m_currentTimer = m_frameTime; }

	//Definir l'animation à jouer en fonction de son nom (ex: "idle", "walk", etc.)
	void SetAnimationByName(const std::string& p_name);

	//Obtenir l'index de la frame courante dans le sprite sheet
	int GetCurrentFrame() const { return m_currentFrame; }

	//Obtenir le nombre total de frames dans le sprite sheet
	int GetTotalFrames() const { return m_totalFrames; }

	//obtenir le nom de l'animation qui est jouee actuellement
	std::string GetNameCurrentAnimation() const { return m_nameAnimation; }

	void SetHasAnimation(bool p_hasAnimation) { m_hasAnimation = p_hasAnimation; }

};
