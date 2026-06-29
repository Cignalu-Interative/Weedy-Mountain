#include "pch.h"
#include "Text.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"
#include "Debug.h"
#include "Utils.h"
#include "FontManager.h"

void Text::CreateTexture()
{
	int textSizeW, textSizeH;
	if (TTF_SizeText(m_font, m_text.c_str(), &textSizeW, &textSizeH) == -1)
	{
		SDL_Log("Lib/Text.cpp/CreateTexture/TTF_GetStringSize/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}

	if (m_textTexture != nullptr)
	{
		SDL_DestroyTexture(m_textTexture);
		m_textTexture = nullptr;
	}
	m_textTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, textSizeW, textSizeH);
	if (m_textTexture == NULL)
	{
		SDL_Log("Lib/Debug.cpp/CreateTexture/SDL_CreateTexture/SDL_CreateTexture non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetTextureBlendMode(m_textTexture, SDL_BLENDMODE_BLEND) < 0)
	{
		SDL_Log("Lib/Debug.cpp/CreateTexture/SDL_SetTextureBlendMode/SDL_SetTextureBlendMode non reussie : %s", SDL_GetError());
		return;
	}
}

Text::Text(const std::string& p_idFont, int p_sizeFont)
{
	m_font = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	m_fontPath = FontManager::Get()->GetFont(p_idFont);
	m_fontSize = p_sizeFont;
	if (m_font == NULL)
	{
		SDL_Log("Lib/Text.cpp/Text/TTF_OpenFont/La font n'a pas pu etre ouverte : %s", TTF_GetError());
		return;
	}
}

Text::~Text()
{
	TTF_CloseFont(m_font);
	SDL_DestroyTexture(m_textTexture);
}

void Text::Initialize(Vector2f p_position, const std::string p_text, Color p_color)
{
	SDL_Log("renderer=%p", m_renderer);

	SetColor(p_color);
	if (p_text == "")
	{
		m_text = " ";
	}
	else
	{
		m_text = p_text;
	}

	CreateTexture();

	UpdateTexture();

	SetPosition(p_position);
}

void Text::SetFont(const std::string& p_idFont, int p_sizeFont)
{
	TTF_Font* newFont = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	if (newFont == NULL)
	{
		SDL_Log("Lib/Text.cpp/SetFont/TTF_OpenFont/La font n'a pas pu etre ouverte : %s", TTF_GetError());
		return;
	}
	m_font = newFont;
	m_fontPath = FontManager::Get()->GetFont(p_idFont);
	UpdateTexture();
	SetPosition(m_positionWanted);
}

void Text::SetText(const std::string& p_text)
{
	m_timeToUpdate -= Debug::Get()->GetDeltaTime();
	if (m_timeToUpdate > m_updateInterval)
	{
		return; // Pas encore le moment de mettre � jour la texture
	}
	m_timeToUpdate = m_updateInterval;
	if (p_text == m_text)
	{
		return; // Le texte n'a pas chang�, pas besoin de recr�er la texture
	}
	if (m_textTexture == nullptr)
	{
		Initialize({ 0.0f, 0.0f }, p_text, { 255, 255, 255, 255 });
	}
	//std::cerr << "Mise a jour du texte : " << p_text << std::endl;
	m_text = p_text;
	if (m_text == "")
	{
		m_text = " ";
	}
	UpdateTexture();
	SetPosition(m_positionWanted);
}

void Text::SetFontSize(int p_size)
{
	if (p_size == m_fontSize)
	{
		return; // La taille n'a pas chang�, pas besoin de recr�er la texture
	}

	m_fontSize = p_size;

	if (m_font != nullptr)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}

	m_font = TTF_OpenFont(m_fontPath.c_str(), m_fontSize);
	if (m_font == nullptr)
	{
		SDL_Log("Text::SetSize/TTF_OpenFont failed : %s", TTF_GetError());
		return;
	}

	UpdateTexture();
	SetPosition(m_positionWanted);
}

void Text::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	if (GetIsVisible() == false)
	{
		return;
	}

	SDL_Renderer* renderer = f_render->m_renderer;
	if (m_textTexture == nullptr)
	{
		return;
	}

	Uint8 r, g, b, a;
	if (SDL_GetTextureColorMod(m_textTexture, &r, &g, &b) < 0)
	{
		SDL_Log("Lib/Text.cpp/Draw/SDL_GetTextureColorMod/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}
	if (SDL_GetTextureAlphaMod(m_textTexture, &a) == false)
	{
		SDL_Log("Lib/Text.cpp/Draw/SDL_GetTextureAlphaMod/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}

	if (r != m_color.r || g != m_color.g || b != m_color.b || a != m_color.a)
	{
		if (SDL_SetTextureColorMod(m_textTexture, m_color.r, m_color.g, m_color.b) < 0)
		{
			SDL_Log("Lib/Text.cpp/Draw/SDL_SetTextureColorMod/cela na pas fonctionn� : %s", SDL_GetError());
			return;
		}
		if (SDL_SetTextureAlphaMod(m_textTexture, m_color.a) < 0)
		{
			SDL_Log("Lib/Text.cpp/Draw/SDL_SetTextureAlphaMod/cela na pas fonctionn� : %s", SDL_GetError());
			return;
		}
	}
	Vector2f screenPos;
	Vector2f dimension;
	if (m_isGUI == true)
	{
		screenPos = GetPosition({ 0.0f, 0.0f });
		dimension = (Vector2f)GetDimension();
	}
	else
	{
		screenPos = p_camera->WorldToScreen(GetPosition({ 0.0f, 0.0f }));
		dimension = Vector2f(GetDimension()) * p_camera->GetZoom();
	}

	Vector2i dimensionWindow = f_window->GetWindowSize();

	if (screenPos.GetX() + dimension.GetX() < 0 || screenPos.GetX() > dimensionWindow.GetX() ||
		screenPos.GetY() + dimension.GetY() < 0 || screenPos.GetY() > dimensionWindow.GetY())
	{
		//std::cerr << "no draw" << std::endl;
		return;
	}

	m_textRect = {
		screenPos.GetX(),
		screenPos.GetY(),
		dimension.GetX(),
		dimension.GetY() };

	float width = dimension.GetX();
	float height = dimension.GetY();
	Vector2f rotPoint = GetRotationPoint();           // ex: (0,0) ou (0.5,0.5)
	SDL_FPoint rotationCenter{
		width * rotPoint.GetX(),                      // 0..width
		height * rotPoint.GetY()                       // 0..height
	};

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (SDL_RenderCopyExF(renderer, m_textTexture,
		NULL,           // Frame du sprite sheet (ou NULL)
		&m_textRect,          // Position/taille � l'�cran
		GetAngle(),        // Angle Transformable (degr�s)
		&rotationCenter,   // Centre rotation pr�-calcul�
		flip) == false)       //  miroir (optionnel)
	{
		SDL_Log("Lib/Sprite.cpp/Draw/SDL_RenderCopyExF/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}

	/*if (SDL_RenderCopyF(renderer, m_textTexture, NULL, &m_textRect) < 0)
	{
		SDL_Log("Lib/Text.cpp/Draw/SDL_RenderCopyF/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}*/

}

void Text::UpdateTexture()
{
	if (m_textTexture == nullptr)
	{
		return;
	}

	int textSizeW, textSizeH;
	if (TTF_SizeText(m_font, m_text.c_str(), &textSizeW, &textSizeH) == -1)
	{
		SDL_Log("Lib/Text.cpp/Update/TTF_GetStringSize/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}
	int sizetetureX = GetDimension().GetX();
	int sizetextureY = GetDimension().GetY();

	if (sizetetureX > 0 || sizetextureY > 0)
	{
		if (textSizeW != sizetetureX || textSizeH != sizetextureY)
		{
			CreateTexture();
		}
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(m_font, m_text.c_str(), { 255, 255, 255, 255 });
	if (textSurface == NULL)
	{
		SDL_Log("Lib/Debug.cpp/Update/TTF_RenderText_Blended/TTF_RenderText_Blended non reussie : %s", TTF_GetError());
		return;
	}

	SDL_PropertiesID props = SDL_GetTextureProperties(m_textTexture);

	int format = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, 0);
	int access = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_ACCESS_NUMBER, 0);
	int width = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
	int height = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

	std::vector<Uint32> pixels(width * height, 0);
	if (SDL_UpdateTexture(m_textTexture, NULL, pixels.data(), width * sizeof(Uint32)) == false)
	{
		SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}

	if (SDL_UpdateTexture(m_textTexture, NULL, textSurface->pixels, textSurface->pitch) == false)
	{
		SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}
	SDL_DestroySurface(textSurface);

	float textWidth, textHeight;
	if (SDL_GetTextureSize(m_textTexture, &textWidth, &textHeight) == false)
	{
		SDL_Log("Lib/Debug.cpp/Update/SDL_QueryTexture/SDL_QueryTexture non reussie : %s", SDL_GetError());
		return;
	}
	std::vector<Uint32> pixels(width * height, 0);
	if (SDL_UpdateTexture(m_textTexture, NULL, pixels.data(), width * sizeof(Uint32)) < 0)
	{
		SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}

	if (SDL_UpdateTexture(m_textTexture, NULL, textSurface->pixels, textSurface->pitch) < 0)
	{
		SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
		return;
	}
	SDL_FreeSurface(textSurface);

	int textWidth, textHeight;
	if (SDL_QueryTexture(m_textTexture, NULL, NULL, &textWidth, &textHeight) < 0)
	{
		SDL_Log("Lib/Debug.cpp/Update/SDL_QueryTexture/SDL_QueryTexture non reussie : %s", SDL_GetError());
		return;
	}
	SetDimension({ textWidth, textHeight });
	//std::cerr << "Taille texture pour le text : " << textSizeW << " " << textSizeH << std::endl;
}

void Text::SetPosition(const Vector2f& p_position, Vector2f p_anchor)
{
	Transformable::SetPosition(p_position, p_anchor);
	m_positionWanted = p_position;
}