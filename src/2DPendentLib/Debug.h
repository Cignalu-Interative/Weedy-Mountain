#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include "Vector2.h"
#include "Color.h"
#include "Drawable.h"
#include "Transformable.h"

#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Text.h"

#include "Camera.h"
//Version 6.0.0

class Render;
class Window;

enum class Type
{
	Point,
	Line,
	Rect,
	FillRect,
	Circle,
	FillCircle,
	Text,

	COUNT
};

class Debug : public Drawable//, public Transformable
{
private:

	SDL_Renderer* m_renderer = nullptr; // Renderer utilisé pour dessiner les éléments de debug

	int m_margin = 5; // Marge en pixels entre le texte de debug et les bords de l'écran

	std::unordered_map<std::string, size_t> m_points; // Map pour stocker les points à dessiner, indexés par un identifiant unique
	std::unordered_map<std::string, size_t> m_lines; // Map pour stocker les lignes à dessiner, indexés par un identifiant unique
	std::unordered_map<std::string, size_t> m_rects; // Map pour stocker les rectangles à dessiner, indexés par un identifiant unique
	std::unordered_map<std::string, size_t> m_fillRects; // Map pour stocker les rectangles remplis à dessiner, indexés par un identifiant unique
	std::unordered_map<std::string, size_t> m_circles; // Map pour stocker les cercles à dessiner, indexés par un identifiant unique
	std::unordered_map<std::string, size_t> m_fillCircles; // Map pour stocker les cercles remplis à dessiner, indexés par un identifiant unique
	std::unordered_map<std::string, size_t> m_texts; // Map pour stocker les textes à dessiner, indexés par un identifiant unique

	std::vector<Point*> m_pointsVector; // Vector pour stocker les points à dessiner, utilisé pour itérer plus rapidement lors du dessin	
	std::vector<Line*> m_linesVector; // Vector pour stocker les lignes à dessiner, utilisé pour itérer plus rapidement lors du dessin
	std::vector<Rectangle*> m_rectsVector; // Vector pour stocker les rectangles à dessiner, utilisé pour itérer plus rapidement lors du dessin
	std::vector<Rectangle*> m_fillRectsVector; // Vector pour stocker les rectangles remplis à dessiner, utilisé pour itérer plus rapidement lors du dessin
	std::vector<Circle*> m_circlesVector; // Vector pour stocker les cercles à dessiner, utilisé pour itérer plus rapidement lors du dessin
	std::vector<Circle*> m_fillCirclesVector; // Vector pour stocker les cercles remplis à dessiner, utilisé pour itérer plus rapidement lors du dessin
	std::vector<Text*> m_textsVector; // Vector pour stocker les textes à dessiner, utilisé pour itérer plus rapidement lors du dessin

	size_t m_pointCount = 0; // Compteur pour générer des identifiants uniques pour les points
	size_t m_lineCount = 0; // Compteur pour générer des identifiants uniques pour les lignes
	size_t m_rectCount = 0; // Compteur pour générer des identifiants uniques pour les rectangles
	size_t m_fillRectCount = 0; // Compteur pour générer des identifiants uniques pour les rectangles remplis
	size_t m_circleCount = 0; // Compteur pour générer des identifiants uniques pour les cercles
	size_t m_fillCircleCount = 0; // Compteur pour générer des identifiants uniques pour les cercles remplis
	size_t m_textCount = 0; // Compteur pour générer des identifiants uniques pour les textes

	// mutable permet de modifier les éléments de debug même dans les fonctions const,
	// ce qui est utile pour les fonctions Get qui retournent des pointeurs vers les éléments de debug
	mutable std::vector<Point*> m_autoPointsCache;
	mutable std::vector<Line*> m_autoLinesCache; 
	mutable std::vector<Rectangle*> m_autoRectsCache;
	mutable std::vector<Rectangle*> m_autoFillRectsCache;
	mutable std::vector<Circle*> m_autoCirclesCache;
	mutable std::vector<Circle*> m_autoFillCirclesCache;
	mutable std::vector<Text*> m_autoTextsCache;

	TTF_TextEngine* m_textEngine = nullptr; // Moteur de rendu de texte pour les textes de debug

	SDL_PropertiesID m_textEnginePropertiesID = 0; // ID des propriétés du moteur de rendu de texte pour les textes de debug

public:

	// Singleton de Debug
	static Debug* Get();

	// Constructeur de Debug
	Debug();
	// Destructeur de Debug
	~Debug();

	void SetRenderer(SDL_Renderer* p_renderer);

	bool CreateTextEngineProperties();

	SDL_Renderer* SDL_GetRenderer() const { return m_renderer; }

	TTF_TextEngine* GetTextEngine() const { return m_textEngine; }

	using DeltaTimeCallback = std::function<float()>;

	static void SetDeltaTimeCallback(DeltaTimeCallback p_callback);
	static float GetDeltaTime();

	// Fonctions pour créer les éléments de debug à dessiner
	// Peut prendre un identifiant unique en paramètre, sinon il sera généré automatiquement
	// Ne pas appeller cette fonction sans id dans une boucle, sinon elle va créer un nouvel élément à chaque frame
	// Si il ya un id au deusieme appel, alors l'élément sera mis à jour avec les nouvelles valeurs
	Point* CreatePoint(std::string p_id, Vector2f p_position, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer une ligne à dessiner
	Line* CreateLine(std::string p_id, Vector2f p_start, Vector2f p_end, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un rectangle à dessiner
	Rectangle* CreateRect(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un rectangle rempli à dessiner
	Rectangle* CreateFillRect(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un cercle à dessiner
	Circle* CreateCircle(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un cercle rempli à dessiner
	Circle* CreateFillCircle(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);
	// Fonction pour créer un texte à dessiner
	//faire un create qui renvoie une addresse de text pour l'update
	Text* CreateText(std::string p_id, Vector2f p_position, const std::string& p_text, Color p_color, bool p_isGUI = false, bool p_clampToScreen = true);

	// Fonctions pour détruire un point de debug grace à son identifiant unique
	void destroyPoint(const std::string& p_id);
	// Fonction pour détruire une ligne de debug grace à son identifiant unique
	void destroyLine(const std::string& p_id);
	// Fonction pour détruire un rectangle de debug grace à son identifiant unique
	void destroyRect(const std::string& p_id);
	// Fonction pour détruire un rectangle remplis de debug grace à son identifiant unique
	void destroyFillRect(const std::string& p_id);
	// Fonction pour détruire un cercle de debug grace à son identifiant unique
	void destroyCircle(const std::string& p_id);
	// Fonction pour détruire un cercle remplis de debug grace à son identifiant unique
	void destroyFillCircle(const std::string& p_id);
	// Fonction pour détruire un texte de debug grace à son identifiant unique
	void destroyText(const std::string& p_id);

	void Destroy(const std::string& p_id);

	// Fonctions pour effacer tous les points de debug
	void ClearPoints();
	// Fonction pour effacer toutes les lignes de debug
	void ClearLines();
	// Fonction pour effacer tous les rectangles de debug
	void ClearRects();
	// Fonction pour effacer tous les rectangles remplis de debug
	void ClearFillRects();
	// Fonction pour effacer tous les cercles de debug
	void ClearCircles();
	// Fonction pour effacer tous les cercles remplis de debug
	void ClearFillCircles();
	// Fonction pour effacer tous les textes de debug
	void ClearTexts();

	void ClearAll();

	// Fonctions pour obtenir les éléments de debug à dessiner grace à leur identifiant unique
	Point* GetPoint(const std::string& p_id);
	// Fonction pour obtenir une ligne de debug grace à son identifiant unique
	Line* GetLine(const std::string& p_id);
	// Fonction pour obtenir un rectangle de debug grace à son identifiant unique
	Rectangle* GetRect(const std::string& p_id);
	// Fonction pour obtenir un rectangle remplis de debug grace à son identifiant unique
	Rectangle* GetFillRect(const std::string& p_id);
	// Fonction pour obtenir un cercle de debug grace à son identifiant unique
	Circle* GetCircle(const std::string& p_id);
	// Fonction pour obtenir un cercle remplis de debug grace à son identifiant unique
	Circle* GetFillCircle(const std::string& p_id);
	// Fonction pour obtenir un texte de debug grace à son identifiant unique
	Text* GetText(const std::string& p_id);

	template<typename T>
	T* GetGeometry(const std::string& p_id)
	{
		T* geometry = nullptr;

		geometry = GetPoint(p_id);
		if (geometry)
			return geometry;

		geometry = GetLine(p_id);
		if (geometry)
			return geometry;

		geometry = GetRect(p_id);
		if (geometry)
			return geometry;

		geometry = GetFillRect(p_id);
		if (geometry)
			return geometry;

		geometry = GetCircle(p_id);
		if (geometry)
			return geometry;

		geometry = GetFillCircle(p_id);
		if (geometry)
			return geometry;

		geometry = GetText(p_id);
		if (geometry)
			return geometry;

		return nullptr;
	}

	std::vector<Point*> GetAllAutoPoints() const;
	std::vector<Line*> GetAllAutoLines() const;
	std::vector<Rectangle*> GetAllAutoRects() const;
	std::vector<Rectangle*> GetAllAutoFillRects() const;
	std::vector<Circle*> GetAllAutoCircles() const;
	std::vector<Circle*> GetAllAutoFillCircles() const;
	std::vector<Text*>& GetAllAutoTexts() const;

	std::vector<Point*>& GetAllPoints();
	std::vector<Line*>& GetAllLines();
	std::vector<Rectangle*>& GetAllRects();
	std::vector<Rectangle*>& GetAllFillRects();
	std::vector<Circle*>& GetAllCircles();
	std::vector<Circle*>& GetAllFillCircles();
	std::vector<Text*>& GetAllTexts();

	// Fonction pour dessiner les éléments de debug à l'écran
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

	// Fonctions pour dessiner les différents types d'éléments de debug
	void DrawPoint(Render* f_render, Window* f_window, Camera* p_camera);
	// Fonction pour dessiner les lignes de debug
	void DrawLine(Render* f_render, Window* f_window, Camera* p_camera);
	// Fonction pour dessiner les rectangles de debug
	void DrawRect(Render* f_render, Window* f_window, Camera* p_camera);
	// Fonction pour dessiner les rectangles remplis de debug
	void DrawFillRect(Render* f_render, Window* f_window, Camera* p_camera);
	// Fonction pour dessiner les cercles de debug
	void DrawCircle(Render* f_render, Window* f_window, Camera* p_camera);
	// Fonction pour dessiner les cercles remplis de debug
	void DrawFillCircle(Render* f_render, Window* f_window, Camera* p_camera);
	// Fonction pour dessiner les textes de debug
	void DrawText(Render* f_render, Window* f_window, Camera* p_camera);

private:

	inline static DeltaTimeCallback m_deltaTimeCallback = nullptr;
};