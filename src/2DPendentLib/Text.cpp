#include "pch.h"
#include "Text.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"
#include "Debug.h"
#include "Utils.h"
#include "FontManager.h"

void Text::CreateTexture(Vector2i p_dimention)
{
	if (m_textTexture != nullptr)
	{
		SDL_DestroyTexture(m_textTexture);
		m_textTexture = nullptr;
	}

	m_textTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p_dimention.GetX(), p_dimention.GetY());
	if (m_textTexture == NULL)
	{
		SDL_Log("Lib/Text.cpp/CreateTexture/SDL_CreateTexture/SDL_CreateTexture non reussie : %s", SDL_GetError());
		return;
	}
	if (SDL_SetTextureBlendMode(m_textTexture, SDL_BLENDMODE_BLEND) == false)
	{
		SDL_Log("Lib/Text.cpp/CreateTexture/SDL_SetTextureBlendMode/SDL_SetTextureBlendMode non reussie : %s", SDL_GetError());
		return;
	}
}

Text::Text(const std::string& p_idFont, int p_sizeFont)
{
	m_font = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	if (m_font == NULL)
	{
		SDL_Log("Lib/Text.cpp/Text/TTF_OpenFont/La font n'a pas pu etre ouverte : %s", SDL_GetError());
		return;
	}
	m_fontPath = FontManager::Get()->GetFont(p_idFont);
	m_fontSize = p_sizeFont;
}

Text::~Text()
{
	if (m_font != nullptr)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	if (m_textTexture != nullptr)
	{
		SDL_DestroyTexture(m_textTexture);
		m_textTexture = nullptr;
	}
}

void Text::Initialize(Vector2f p_position, const std::string p_text, Color p_color, TypeText p_typeText, bool p_wrapped)
{
	if (p_text == "")
	{
		m_text = " ";
	}
	else
	{
		m_text = p_text;
	}

	m_typeText = p_typeText;

	m_isWrapped = p_wrapped;

	SetColor(p_color);

	CreateTexture({1, 1});

	UpdateTexture();

	SetPosition(p_position);
}

void Text::SetFont(const std::string& p_idFont, int p_sizeFont)
{
	TTF_Font* newFont = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	if (newFont == NULL)
	{
		SDL_Log("Lib/Text.cpp/SetFont/TTF_OpenFont/La font n'a pas pu etre ouverte : %s", SDL_GetError());
		return;
	}
	if (m_font != nullptr)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
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
	m_text = p_text;
	if (m_text == "")
	{
		m_text = " ";
	}
	if (m_textTexture == nullptr)
	{
		Initialize({ 0.0f, 0.0f }, p_text, { 255, 255, 255, 255 });
	}
	else
	{
		UpdateTexture();
	}
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
		SDL_Log("Text::SetSize/TTF_OpenFont failed : %s", SDL_GetError());
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
	if (SDL_GetTextureColorMod(m_textTexture, &r, &g, &b) == false)
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
		if (SDL_SetTextureColorMod(m_textTexture, m_color.r, m_color.g, m_color.b) == false)
		{
			SDL_Log("Lib/Text.cpp/Draw/SDL_SetTextureColorMod/cela na pas fonctionn� : %s", SDL_GetError());
			return;
		}
		if (SDL_SetTextureAlphaMod(m_textTexture, m_color.a) == false)
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

	SDL_FlipMode flip = SDL_FLIP_NONE;

	if (SDL_RenderTextureRotated(renderer, m_textTexture,
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

	TTF_SetFontOutline(m_font, 0); // On remet l'outline à 0 pour calculer la taille du texte sans l'outline

	size_t lenText = strlen(m_text.c_str());

	/*int textSizeW, textSizeH;
	if (m_isWrapped)
	{
		if (TTF_GetStringSizeWrapped(m_font, m_text.c_str(), lenText, m_wrapWidth, &textSizeW, &textSizeH) == false)
		{
			SDL_Log("Lib/Text.cpp/Update/TTF_GetStringSizeWrapped/cela na pas fonctionn� : %s", SDL_GetError());
			return;
		}
	}
	else
	{
		if (TTF_GetStringSize(m_font, m_text.c_str(), lenText, &textSizeW, &textSizeH) == false)
		{
			SDL_Log("Lib/Text.cpp/Update/TTF_GetStringSize/cela na pas fonctionn� : %s", SDL_GetError());
			return;
		}
	}*/
	
	/*int sizetetureX = GetDimension().GetX();
	int sizetextureY = GetDimension().GetY();

	if (sizetetureX > 0 || sizetextureY > 0)
	{
		if (textSizeW != sizetetureX || textSizeH != sizetextureY)
		{
			CreateTexture();
		}
	}*/

	//int finalW = textSizeW;
	//int finalH = textSizeH;
	//if (m_outlineSize > 0)
	//{
	//	finalW += m_outlineSize * 2;
	//	finalH += m_outlineSize * 2;
	//}

	//int sizetetureX = GetDimension().GetX();
	//int sizetextureY = GetDimension().GetY();

	//if (sizetetureX > 0 || sizetextureY > 0)
	//{
	//	if (finalW != sizetetureX || finalH != sizetextureY) // <-- finalW/finalH, pas textSizeW/textSizeH
	//	{
	//		CreateTexture();
	//	}
	//}

	auto RenderPass = [&](const Color& p_color) -> SDL_Surface*
		{
			SDL_Color sdlColor{ p_color.r, p_color.g, p_color.b, p_color.a };

			switch (m_typeText)
			{
			case Solid:
				return m_isWrapped
					? TTF_RenderText_Solid_Wrapped(m_font, m_text.c_str(), lenText, sdlColor, m_wrapWidth)
					: TTF_RenderText_Solid(m_font, m_text.c_str(), lenText, sdlColor);

			case Shaded:
			{
				SDL_Color backgroundColor{ m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a };
				return m_isWrapped
					? TTF_RenderText_Shaded_Wrapped(m_font, m_text.c_str(), lenText, sdlColor, backgroundColor, m_wrapWidth)
					: TTF_RenderText_Shaded(m_font, m_text.c_str(), lenText, sdlColor, backgroundColor);
			}

			case Blended:
				return m_isWrapped
					? TTF_RenderText_Blended_Wrapped(m_font, m_text.c_str(), lenText, sdlColor, m_wrapWidth)
					: TTF_RenderText_Blended(m_font, m_text.c_str(), lenText, sdlColor);
			}
			return nullptr;
		};

	TTF_SetFontOutline(m_font, 0);
	SDL_Surface* textSurface = RenderPass(m_color);
	if (textSurface == nullptr)
	{
		SDL_Log("Lib/Debug.cpp/Update/RenderPass/textSurface non reussie : %s", SDL_GetError());
		return;
	}
	SDL_Surface* convertedSurface = nullptr;
	if (m_outlineSize <= 0)
	{
		convertedSurface = SDL_ConvertSurface(textSurface, SDL_PIXELFORMAT_RGBA32);
		SDL_DestroySurface(textSurface);
		if (convertedSurface == nullptr)
		{
			SDL_Log("Lib/Debug.cpp/Update/SDL_ConvertSurface/convertedSurface non reussie : %s", SDL_GetError());
			return;
		}
	}
	else
	{
		TTF_SetFontOutline(m_font, m_outlineSize);
		SDL_Surface* outlineSurface = RenderPass(m_outlineColor);
		TTF_SetFontOutline(m_font, 0);

		if (outlineSurface == nullptr)
		{
			SDL_Log("Lib/Debug.cpp/Update/RenderPass/outlineSurface non reussie : %s", SDL_GetError());
			SDL_DestroySurface(textSurface);
			return;
		}

		SDL_Surface* textRGBA = SDL_ConvertSurface(textSurface, SDL_PIXELFORMAT_RGBA32);
		if (textRGBA == nullptr)
		{
			SDL_Log("Lib/Debug.cpp/Update/SDL_ConvertSurface/textRGBA non reussie : %s", SDL_GetError());
			SDL_DestroySurface(textSurface);
			SDL_DestroySurface(outlineSurface);
			return;
		}
		SDL_Surface* outlineRGBA = SDL_ConvertSurface(outlineSurface, SDL_PIXELFORMAT_RGBA32);
		if (outlineRGBA == nullptr)
		{
			SDL_Log("Lib/Debug.cpp/Update/SDL_ConvertSurface/outlineRGBA non reussie : %s", SDL_GetError());
			SDL_DestroySurface(textSurface);
			SDL_DestroySurface(outlineSurface);
			SDL_DestroySurface(textRGBA);
			return;
		}
		SDL_DestroySurface(textSurface);
		SDL_DestroySurface(outlineSurface);

		if (SDL_SetSurfaceBlendMode(textRGBA, SDL_BLENDMODE_BLEND) == false)
		{
			SDL_Log("Lib/Debug.cpp/Update/SDL_SetSurfaceBlendMode non reussie : %s", SDL_GetError());
			SDL_DestroySurface(textRGBA);
			SDL_DestroySurface(outlineRGBA);
			return;
		}

		SDL_Rect dstRect{ m_outlineSize, m_outlineSize, textRGBA->w, textRGBA->h };
		if (SDL_BlitSurface(textRGBA, nullptr, outlineRGBA, &dstRect) == false)
		{
			SDL_Log("Lib/Debug.cpp/Update/SDL_BlitSurface non reussie : %s", SDL_GetError());
			SDL_DestroySurface(textRGBA);
			SDL_DestroySurface(outlineRGBA);
			return;
		}

		convertedSurface = outlineRGBA;
		SDL_DestroySurface(textRGBA);
		SDL_DestroySurface(outlineRGBA);
	}

	float currentW, currentH;
	SDL_GetTextureSize(m_textTexture, &currentW, &currentH);
	if ((int)currentW != convertedSurface->w || (int)currentH != convertedSurface->h)
	{
		CreateTexture({ convertedSurface->w, convertedSurface->h });
		if (m_textTexture == nullptr)
		{
			SDL_DestroySurface(convertedSurface);
			return;
		}
	}

	//SDL_PropertiesID props = SDL_GetTextureProperties(m_textTexture);
	//if (props == 0)
	//{
	//	SDL_Log("Lib/Debug.cpp/Update/SDL_GetTextureProperties/SDL_GetTextureProperties non reussie : %s", SDL_GetError());
	//	return;
	//}

	//int format = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, 0);
	//int access = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_ACCESS_NUMBER, 0);
	//int width = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
	//int height = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

	//std::vector<Uint32> pixels(width * height, 0); // Crée un vecteur de pixels initialisé à zéro (transparent)
	//if (SDL_UpdateTexture(m_textTexture, NULL, pixels.data(), width * sizeof(Uint32)) == false)
	//{
	//	SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
	//	return;
	//}

	if (SDL_UpdateTexture(m_textTexture, NULL, convertedSurface->pixels, convertedSurface->pitch) == false)
	{
		SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
		SDL_DestroySurface(convertedSurface);
		return;
	}
	SDL_DestroySurface(convertedSurface);

	float textWidth, textHeight;
	if (SDL_GetTextureSize(m_textTexture, &textWidth, &textHeight) == false)
	{
		SDL_Log("Lib/Debug.cpp/Update/SDL_QueryTexture/SDL_QueryTexture non reussie : %s", SDL_GetError());
		return;
	}
	SetDimension({ (int)textWidth, (int)textHeight });
	//std::cerr << "Taille texture pour le text : " << textSizeW << " " << textSizeH << std::endl;
}

void Text::SetPosition(const Vector2f& p_position, Vector2f p_anchor)
{
	Transformable::SetPosition(p_position, p_anchor);
	m_positionWanted = p_position;
}

void Text::SetFontStyle(bool p_bold, bool p_italic, bool p_underline, bool p_strikethrough)
{
	TTF_FontStyleFlags style = TTF_STYLE_NORMAL;
	int verif = 0;

	if (p_bold)
	{
		style |= TTF_STYLE_BOLD;
		verif += 1;
	}
	if (p_italic)
	{
		style |= TTF_STYLE_ITALIC;
		verif += 2;
	}
	if (p_underline)
	{
		style |= TTF_STYLE_UNDERLINE;
		verif += 4;
	}
	if (p_strikethrough)
	{
		style |= TTF_STYLE_STRIKETHROUGH;
		verif += 8;
	}

	if (verif != style)
	{
		SDL_Log("Lib/Text.cpp/SetFontStyle/Erreur dans la combinaison des styles de police");
		return;
	}
	
	m_fontStyle = style;

	TTF_SetFontStyle(m_font, m_fontStyle);

	UpdateTexture();
}

void Text::SetOutline(int p_outlineSize)
{
	if (p_outlineSize < 0)
	{
		m_isHasOutline = false;
		SDL_Log("Lib/Text.cpp/SetOutline/La taille du contour ne peut pas etre negative");
		return;
	}
	else if (p_outlineSize > 0)
	{
		m_isHasOutline = true;
		m_outlineSize = p_outlineSize;
	}
	else
	{
		m_isHasOutline = false;
	}
}