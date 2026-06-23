#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "Vector2.h"
#include "Geometry.h"
#include "Drawable.h"

class Text : public Geometry, public Drawable
{
private:

	std::string m_fontPath; // Chemin vers le fichier de police utilisé pour le texte
	std::string m_text; // Texte à afficher

	TTF_Font* m_font = nullptr; // Police de caractères utilisée pour le texte

	SDL_Texture* m_textTexture = nullptr; // Texture du texte

	SDL_Renderer* m_renderer = nullptr; // Renderer utilisé pour dessiner le texte

	SDL_FRect m_textRect; // Rectangle du texte

	int m_fontSize; // Taille de la police utilisée pour le texte

	float m_timeToUpdate = 0.0f; // Temps écoulé depuis la dernière mise à jour de la texture du texte
	float m_updateInterval = 0.5f; // Intervalle de temps entre les mises à jour de la texture du texte (en secondes)

	bool m_clampToScreen = false; // Si true, la geometry sera dessiné à la position la plus proche sur l'écran si sa position dépasse les limites de l'écran

	void CreateTexture();

	Vector2f m_positionWanted; //Position voulue pour le texte, indépendante de la position réelle du texte avec la taille du texte

public:

	Text(const std::string& p_idFont = "DefaultPolice", int p_sizeFont = 28);
	~Text();

	void Initialize(Vector2f p_position, const std::string p_text, Color p_color);

	void SetFont(const std::string& p_idFont, int p_sizeFont);
	TTF_Font* GetFont() const { return m_font; }

	void SetText(const std::string& p_text);
	std::string GetText() const { return m_text; }

	void SetFontSize(int p_size);
	int GetFontSize() const { return m_fontSize; }

	void SetTimeToUpdate(float p_timeToUpdate) { m_updateInterval = p_timeToUpdate; }
	float GetTimeToUpdate() const { return m_updateInterval; }

	void SetRenderer(SDL_Renderer* p_renderer) { m_renderer = p_renderer; }
	SDL_Renderer* GetRenderer() const { return m_renderer; }

	void SetIsClamp(bool p_isClamp) { m_clampToScreen = p_isClamp; }
	bool GetIsClamp() { return m_clampToScreen; }

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

	void UpdateTexture();

	void SetPosition(const Vector2f& p_position, Vector2f p_anchor = { 0.5f, 0.5f });
};