#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "Vector2.h"
#include "Geometry.h"
#include "Drawable.h"

enum TypeText
{
	Solid,
	Shaded,
	Blended,

	MaxType
};

enum HorizontalAlignment
{
	Invalid = TTF_HORIZONTAL_ALIGN_INVALID,
	Left = TTF_HORIZONTAL_ALIGN_LEFT,
	Center = TTF_HORIZONTAL_ALIGN_CENTER,
	Right = TTF_HORIZONTAL_ALIGN_RIGHT,

	MaxHorizontalAlignment
};

enum FontStyle : TTF_FontStyleFlags
{
	Normal = TTF_STYLE_NORMAL,
	Bold = TTF_STYLE_BOLD,
	Italic = TTF_STYLE_ITALIC,
	Underline = TTF_STYLE_UNDERLINE,
	Strikethrough = TTF_STYLE_STRIKETHROUGH,

	MaxFontStyle
};

class Text : public Geometry, public Drawable
{
private:

	std::string m_fontPath; // Chemin vers le fichier de police utilisé pour le texte
	std::string m_text; // Texte à afficher

	TypeText m_typeText; // Type de rendu du texte (Solid, Shaded, Blended)

	TTF_Font* m_font = nullptr; // Police de caractères utilisée pour le texte

	TTF_TextEngine* m_textEngine = nullptr; // Moteur de rendu de texte utilisé pour le rendu du texte

	TTF_Text* m_dynamicText = nullptr; // Texte dynamique utilisé pour le rendu du texte

	SDL_Texture* m_textTexture = nullptr; // Texture du texte

	SDL_Renderer* m_renderer = nullptr; // Renderer utilisé pour dessiner le texte

	SDL_FRect m_textRect; // Rectangle du texte

	int m_fontSize; // Taille de la police utilisée pour le texte

	float m_timeToUpdate = 0.0f; // Temps écoulé depuis la dernière mise à jour de la texture du texte
	float m_updateInterval = 0.5f; // Intervalle de temps entre les mises à jour de la texture du texte (en secondes)

	bool m_clampToScreen = false; // Si true, la geometry sera dessiné à la position la plus proche sur l'écran si sa position dépasse les limites de l'écran

	void CreateTexture(Vector2i p_dimention);

	Vector2f m_positionWanted; //Position voulue pour le texte, indépendante de la position réelle du texte avec la taille du texte

	bool m_isWrapped = false; // Si true, le texte fera des retours à la ligne automatiques si la largeur du texte dépasse la largeur maximale spécifiée

	int m_wrapWidth = 0; // Largeur maximale du texte avant de faire un retour à la ligne automatique (en pixels)

	Color m_backgroundColor = ColorsNamed::Transparent; // Couleur de fond du texte (par défaut transparent)

	TTF_FontStyleFlags m_fontStyle = TTF_STYLE_NORMAL; // Style de police utilisé pour le texte (par défaut normal)

	bool m_isHasOutline = false; // Si true, le texte aura un contour (outline) autour des caractères

	int m_outlineSize = 0; // Taille du contour (outline) autour des caractères du texte (en pixels)

	Color m_outlineColor = ColorsNamed::Red;

	int m_lineSpacing = 0; // Espacement entre les lignes de texte (en pixels)

public:

	Text(const std::string& p_idFont = "DefaultPolice", int p_sizeFont = 28);
	~Text();

	void SetTextEngine(TTF_TextEngine* p_engine) { m_textEngine = p_engine; }

	void Initialize(Vector2f p_position, const std::string p_text, Color p_color = ColorsNamed::White, TypeText p_typeText = Solid, bool p_wrapped = false);

	void SetFont(const std::string& p_idFont, int p_sizeFont);
	TTF_Font* GetFont() const;

	void SetText(const std::string& p_text);
	std::string GetText() const { return m_text; }

	void SetFontSize(int p_size);
	int GetFontSize() const;

	void SetTimeToUpdate(float p_timeToUpdate) { m_updateInterval = p_timeToUpdate; }
	float GetTimeToUpdate() const { return m_updateInterval; }

	void SetRenderer(SDL_Renderer* p_renderer) { m_renderer = p_renderer; }
	SDL_Renderer* GetRenderer() const { return m_renderer; }

	void SetIsClamp(bool p_isClamp) { m_clampToScreen = p_isClamp; }
	bool GetIsClamp() { return m_clampToScreen; }

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

	void UpdateTexture();

	void SetPosition(const Vector2f& p_position, Vector2f p_anchor = { 0.5f, 0.5f });

	void SetColor(Color p_color);
	Color GetColor() const;

	void SetBackgroundColor(Color p_color) { m_backgroundColor = p_color; }
	Color GetBackgroundColor() const { return m_backgroundColor; }

	void SetIsWrapped(bool p_wrapped) { m_isWrapped = p_wrapped; }
	bool GetIsWrapped() const { return m_isWrapped; }

	void SetWrapWidth(int p_wrapWidth);
	int GetWrapWidth() const;

	void SetTypeText(TypeText p_typeText) { m_typeText = p_typeText; }
	TypeText GetTypeText() const { return m_typeText; }

	void SetFontStyle(bool p_bold = false, bool p_italic = false, bool p_underline = false, bool p_strikethrough = false);
	TTF_FontStyleFlags GetFontStyle() const;

	void SetOutline(int p_outlineSize);

	void SetOutLineColor(Color p_color) { m_outlineColor = p_color; }
	Color GetOutLineColor() const { return m_outlineColor; }

	void SetLineSpacing(int p_lineSpacing);
	int GetLineSpacing() const;

	void HorizontalAlignText(HorizontalAlignment p_alignment);
	HorizontalAlignment GetHorizontalAlignText() const;

	Vector2i GetTextSize() const;

	

};

#include "Text.inl"