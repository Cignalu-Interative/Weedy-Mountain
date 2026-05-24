#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <vector>

class FontManager
{
private:

	std::unordered_map<std::string, size_t> m_fontsMap; // Map pour stocker les fonts chargées, indexées par leur chemin d'accès
	std::vector<std::string> m_fontsVector; // Vector pour stocker les fonts chargées, utilisé pour itérer plus rapidement lors de la libération des ressources
	std::vector<std::string> m_fontIdsVector; // Vector pour stocker les identifiants des fonts chargées, utilisé pour itérer plus rapidement lors de la libération des ressources

public:

	//Singleton de l'FontManager
	static FontManager* Get();

	// Constructeur par défaut de FontManager
	FontManager();
	// Destructeur de FontManager
	~FontManager();

	// Ajoute une font à partir d'un répertoire JSON contenant les chemins d'accès des textures
	void AddFontByRepertory(const std::string& p_path);

	// Ajoute une font à partir d'un chemin d'accès
	void AddFontByPath(const std::string& p_path);

	// Ajoute une font à partir d'un identifiant qui son nom est le même que son chemin d'accès
	void DestroyFont(const std::string& p_id);

	// Libère toutes les fonts chargées
	void ClearFonts();

	// Obtient une font à partir de son identifiant
	const std::string& GetFont(const std::string& p_id) const;

};