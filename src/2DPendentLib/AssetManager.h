#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include <vector>

class Render;
class AssetManager
{
private:

	void SetRenderer(SDL_Renderer* p_renderer) { m_renderer = p_renderer; }

	SDL_Renderer* SDL_GetRenderer() const { return m_renderer; }

	SDL_Renderer* m_renderer = nullptr; // Renderer utilisé pour charger les textures

	std::unordered_map<std::string, size_t> m_texturesMap; // Map pour stocker les textures chargées, indexées par leur chemin d'accès
	std::vector<SDL_Texture*> m_texturesVector; // Vector pour stocker les textures chargées, utilisé pour itérer plus rapidement lors de la libération des ressources
	std::vector<std::string> m_textureIdsVector; // Vector pour stocker les identifiants des textures chargées, utilisé pour itérer plus rapidement lors de la libération des ressources

public:

	//Singleton de l'FontManager
	static AssetManager* Get();

	// Constructeur par défaut de AssetManager
	AssetManager();
	// Destructeur de AssetManager
	~AssetManager();

	// Ajoute une texture à partir d'un répertoire JSON contenant les chemins d'accès des textures
	void AddTextureByRepertory(const std::string& p_path);

	// Ajoute une texture à partir d'un chemin d'accès
	void AddTextureByPath(const std::string& p_path);

	// Ajoute une texture à partir d'un identifiant qui son nom est le même que son chemin d'accès
	void DestroyTexture(const std::string& p_id);

	// Libère toutes les textures chargées
	void ClearTextures();

	// Obtient une texture à partir de son identifiant
	SDL_Texture* GetTexture(const std::string& p_id) const;

	friend class Application;
};

