#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <variant>
#include "BatchCommand.h"
	
using CommandVariant = std::variant<TileTiledCommand, SpriteCommand>;

struct Data
{
	SDL_Texture* s_texture = nullptr;
	std::vector<SDL_Vertex> s_vertices;
	std::vector<int> s_indices;

	Data()
	{
		s_vertices.reserve(10000); // Réserve de l'espace pour 1000 sommets
		s_indices.reserve(15000);  // Réserve de l'espace pour 1500 indices
	}
};

class BatchRenderer2D
{
private:

	SDL_Renderer* m_renderer = nullptr; // Pointeur vers le renderer SDL

	std::vector<CommandVariant> m_commands;

	std::vector<Data> m_batches;

	SDL_Texture* m_lastSizedTexture = nullptr; // cache pour GetTextureSize

	float m_lastTexW = 0.f;
	float m_lastTexH = 0.f;

	template <typename T>
	void AddQuadToBuffer(std::vector<SDL_Vertex>& vertices, const T& p_Command);

public:

	static BatchRenderer2D* Get();

	BatchRenderer2D();
	~BatchRenderer2D();

	void SetRenderer(SDL_Renderer* p_renderer) { m_renderer = p_renderer; }

	// Fonction template pour remplir le batch avec différents types de commandes
	// Elle accepte un pointeur vers une commande de type T (par exemple, TileTiledCommand ou SpriteCommand) et l'ajoute au batch.
	template <typename T>
	void Fill(const T& p_Command);

	// Soumet les commandes au batch pour le rendu
	// Elle envoie toute les données de rendu accumulées dans le batch vers le GPU pour être traitées et affichées à l'écran.
	// Vide le batch et effectue le rendu des commandes soumises
	// Elle vide le batch pour préparer la prochaine série de commandes.
	const std::vector<Data>& Flush();
	 
};

#include "BatchRenderer2D.inl"

