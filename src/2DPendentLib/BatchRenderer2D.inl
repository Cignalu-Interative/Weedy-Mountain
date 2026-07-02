#pragma once
#include "BatchRenderer2D.h"
#include "Utils.h"


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

		SDL_FPoint p1 = {}; // Top-Left
		SDL_FPoint p2 = {}; // Top-Right
		SDL_FPoint p3 = {}; // Bottom-Left
		SDL_FPoint p4 = {}; // Bottom-Right

		if constexpr (std::is_same_v<T, TileTiledCommand>) // Pour ajouter un traitement spécifique pour TileTiledCommand si nécessaire
		{
			p1 = { x1, y1 }; // Top-Left
			p2 = { x2, y1 }; // Top-Right
			p3 = { x1, y2 }; // Bottom-Left
			p4 = { x2, y2 }; // Bottom-Right
		}
		else if constexpr (std::is_same_v<T, SpriteCommand>) // Pour ajouter un traitement spécifique pour SpriteCommand si nécessaire
		{
			Utils::InfoRotate inforotate = Utils::CreateInfoToRotate(
				Vector2f(x1, y1), Vector2f(x2, y2), p_Command.s_angle, Vector2f(p_Command.s_rotationCenter.x, p_Command.s_rotationCenter.y));

			Vector2f v2f1 = Utils::Rotate(Vector2f(x1, y1), inforotate.s_centerQuad, inforotate.s_cos, inforotate.s_sin); // Top-Left
			Vector2f v2f2 = Utils::Rotate(Vector2f(x2, y1), inforotate.s_centerQuad, inforotate.s_cos, inforotate.s_sin); // Top-Right
			Vector2f v2f3 = Utils::Rotate(Vector2f(x1, y2), inforotate.s_centerQuad, inforotate.s_cos, inforotate.s_sin); // Bottom-Left
			Vector2f v2f4 = Utils::Rotate(Vector2f(x2, y2), inforotate.s_centerQuad, inforotate.s_cos, inforotate.s_sin); // Bottom-Right

			p1.x = v2f1.GetX(); p1.y = v2f1.GetY();
			p2.x = v2f2.GetX(); p2.y = v2f2.GetY();
			p3.x = v2f3.GetX(); p3.y = v2f3.GetY();
			p4.x = v2f4.GetX(); p4.y = v2f4.GetY();
		} 

		SDL_Vertex V1 = { p1, white, {u1, v1} };
		SDL_Vertex V2 = { p2, white, {u2, v1} };
		SDL_Vertex V3 = { p3, white, {u1, v2} };
		SDL_Vertex V4 = { p4, white, {u2, v2} };

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
