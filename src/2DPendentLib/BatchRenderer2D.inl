#pragma once
#include "BatchRenderer2D.h"

template<typename T>
inline void BatchRenderer2D::AddQuadToBuffer(std::vector<SDL_Vertex>& vertices, const T& p_Command)
{
	
		// Définition des 4 coins du rectangle (x, y)
		float x1 = p_Command.s_dstRect.x;
		float y1 = p_Command.s_dstRect.y;
		float x2 = p_Command.s_dstRect.x + p_Command.s_dstRect.w;
		float y2 = p_Command.s_dstRect.y + p_Command.s_dstRect.h;

		float tX, tY;
		if (SDL_GetTextureSize(p_Command.s_texture, &tX, &tY) == false)
		{
			SDL_Log("Lib/BatchRenderer2D.inl/BatchRenderer2D::AddQuadToBuffer/SDL_GetTextureSize/cela n'a pas fonctionner : %s", SDL_GetError());
			return;
		}

		// Coordonnées de texture (u, v) - Normalisées entre 0 et 1
		// Note : Tu dois diviser par la taille de la texture totale
		float u1 = p_Command.s_srcRect.x / tX;
		float v1 = p_Command.s_srcRect.y / tY;
		float u2 = (p_Command.s_srcRect.x + p_Command.s_srcRect.w) / tX;
		float v2 = (p_Command.s_srcRect.y + p_Command.s_srcRect.h) / tY;

		SDL_FColor white = { 1, 1, 1, 1 };

		if constexpr (std::is_same_v<T, TileTiledCommand>) // Pour ajouter un traitement spécifique pour TileTiledCommand si nécessaire
		{
		}
		else if constexpr (std::is_same_v<T, SpriteCommand>) // Pour ajouter un traitement spécifique pour SpriteCommand si nécessaire
		{
		} 

		SDL_Vertex V1 = { {x1, y1}, white, {u1, v1} };
		SDL_Vertex V2 = { {x2, y1}, white, {u2, v1} };
		SDL_Vertex V3 = { {x1, y2}, white, {u1, v2} };
		SDL_Vertex V4 = { {x2, y2}, white, {u2, v2} };

		// Création des 4 sommets (pour un rendu en triangle strip)
		vertices.push_back(V1); // Top-Left
		vertices.push_back(V2); // Top-Right
		vertices.push_back(V3); // Bottom-Left
		vertices.push_back(V4); // Bottom-Right
}

template <typename T>
void BatchRenderer2D::Fill(const T& p_Command)
{
	static_assert(std::is_same_v<T, TileTiledCommand> || std::is_same_v<T, SpriteCommand>,
		"Type de commande non pris en charge par BatchRenderer2D::Fill");

	m_commands.push_back(p_Command);
}
