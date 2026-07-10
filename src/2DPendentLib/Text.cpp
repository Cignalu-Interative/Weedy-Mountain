#include "pch.h"
#include "Text.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"
#include "Debug.h"
#include "Utils.h"
#include "FontManager.h"

void Text::CreateText()
{
	size_t lenText = strlen(m_text.c_str());

	m_dynamicText = TTF_CreateText(m_textEngine, m_font, m_text.c_str(), lenText);
	if (m_dynamicText == NULL)
	{
		SDL_Log("Lib/Text.cpp/CreateTexture/TTF_CreateText/TTF_CreateText m_dynamicText non reussie : %s", SDL_GetError());
		return;
	}

	m_outlineText = TTF_CreateText(m_textEngine, m_outlineFont, m_text.c_str(), lenText);
	if (m_outlineText == NULL)
	{
		SDL_Log("Lib/Text.cpp/CreateTexture/TTF_CreateText/TTF_CreateText m_outlineText non reussie : %s", SDL_GetError());
		return;
	}
	SetOutLineColor(m_outlineColor);
	
	SetDimension(GetTextSize());
}

Text::Text(const std::string& p_idFont, int p_sizeFont)
{
	m_font = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	if (m_font == NULL)
	{
		SDL_Log("Lib/Text.cpp/Text/TTF_OpenFont/La font pour m_font n'a pas pu etre ouverte : %s", SDL_GetError());
		return;
	}

	m_outlineFont = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	if (m_outlineFont == NULL)
	{
		SDL_Log("Lib/Text.cpp/Text/TTF_OpenFont/La font pour m_outlineFont n'a pas pu etre ouverte : %s", SDL_GetError());
		return;
	}
}

Text::~Text()
{
	if (m_dynamicText)
	{
		TTF_DestroyText(m_dynamicText);
		m_dynamicText = nullptr;
	}
	if (m_outlineText)
	{
		TTF_DestroyText(m_outlineText);
		m_outlineText = nullptr;
	}
	if (m_font != nullptr)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	if (m_outlineFont != nullptr)
	{
		TTF_CloseFont(m_outlineFont);
		m_outlineFont = nullptr;
	}
}

void Text::Initialize(Vector2f p_position, const std::string p_text, Color p_color)
{
	if (p_text == "")
	{
		m_text = " ";
	}
	else
	{
		m_text = p_text;
	}

	CreateText();

	SetColor(p_color);

	SetPosition(p_position);

	SetDimension(GetTextSize());
}

void Text::SetFont(const std::string& p_idFont, int p_sizeFont)
{
	TTF_Font* newFont = TTF_OpenFont(FontManager::Get()->GetFont(p_idFont).c_str(), p_sizeFont);
	TTF_Font* newOutlineFont = newFont;
	if (!newFont || !newOutlineFont)
	{
		return;
	}

	TTF_SetFontOutline(newOutlineFont, m_outlineSize);

	TTF_SetTextFont(m_dynamicText, newFont);
	TTF_SetTextFont(m_outlineText, newOutlineFont);

	if (m_font)
	{
		TTF_CloseFont(m_font);
	}
	if (m_outlineFont)
	{
		TTF_CloseFont(m_outlineFont);
	}

	m_font = newFont;
	m_outlineFont = newOutlineFont;
	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

TTF_Font* Text::GetFont() const
{
	TTF_Font* font = TTF_GetTextFont(m_dynamicText);
	if (font == NULL)
	{
		SDL_Log("Text::GetFont/TTF_GetTextFont failed : %s", SDL_GetError());
		return nullptr;
	}
	return font;
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

	size_t lenText = strlen(m_text.c_str());

	if (TTF_SetTextString(m_dynamicText, m_text.c_str(), lenText) == false)
	{
		SDL_Log("Text::SetText/TTF_SetTextString m_dynamicText failed : %s", SDL_GetError());
		return;
	}
	if (TTF_SetTextString(m_outlineText, m_text.c_str(), lenText) == false)
	{
		SDL_Log("Text::SetText/TTF_SetTextString m_outlineText failed : %s", SDL_GetError());
		return;
	}
	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

void Text::SetFontSize(int p_size)
{
	if (p_size == m_fontSize)
	{
		return; // La taille n'a pas chang�, pas besoin de recr�er la texture
	}

	m_fontSize = p_size;

	if (TTF_SetFontSize(m_font, m_fontSize) == false)
	{
		SDL_Log("Text::SetFontSize/TTF_SetFontSize pour m_font failed : %s", SDL_GetError());
		return;
	}
	if (TTF_SetFontSize(m_outlineFont, m_fontSize) == false)
	{
		SDL_Log("Text::SetFontSize/TTF_SetFontSize pour m_outlineFont failed : %s", SDL_GetError());
		return;
	}
	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

float Text::GetFontSize() const
{
	float size = TTF_GetFontSize(m_font);
	if (size <= 0.0f)
	{
		SDL_Log("Text::GetFontSize/TTF_GetFontSize failed : %s", SDL_GetError());
		return 0;
	}
	return size; //convertir en float quand je passerais que en dynamique
}


void Text::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	if (m_dynamicText == nullptr || m_outlineText == nullptr)
	{ 
		return;
	}

	if (GetIsVisible() == false)
	{
		return;
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

	if (m_isHasOutline)
	{
		TTF_DrawRendererText(m_outlineText, m_textRect.x, m_textRect.y);
	}
	
	float sizetext = m_outlineSize + m_outlineSize * 0.5f;

	TTF_DrawRendererText(m_dynamicText, m_textRect.x + sizetext, m_textRect.y + sizetext);
}

void Text::UpdateTexture()
{
	//if (m_textTexture == nullptr)
	//{
	//	return;
	//}

	//TTF_SetFontOutline(m_font, 0); // On remet l'outline à 0 pour calculer la taille du texte sans l'outline

	//size_t lenText = strlen(m_text.c_str());

	////TTF_SetFontSDF(m_font, true);

	//auto RenderPass = [&](const Color& p_color) -> SDL_Surface*
	//	{
	//		SDL_Color sdlColor{ p_color.r, p_color.g, p_color.b, p_color.a };

	//		switch (m_typeText)
	//		{
	//		case Solid:
	//			return m_isWrapped
	//				? TTF_RenderText_Solid_Wrapped(m_font, m_text.c_str(), lenText, sdlColor, m_wrapWidth)
	//				: TTF_RenderText_Solid(m_font, m_text.c_str(), lenText, sdlColor);

	//		case Shaded:
	//		{
	//			SDL_Color backgroundColor{ m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a };
	//			return m_isWrapped
	//				? TTF_RenderText_Shaded_Wrapped(m_font, m_text.c_str(), lenText, sdlColor, backgroundColor, m_wrapWidth)
	//				: TTF_RenderText_Shaded(m_font, m_text.c_str(), lenText, sdlColor, backgroundColor);
	//		}

	//		case Blended:
	//			return m_isWrapped
	//				? TTF_RenderText_Blended_Wrapped(m_font, m_text.c_str(), lenText, sdlColor, m_wrapWidth)
	//				: TTF_RenderText_Blended(m_font, m_text.c_str(), lenText, sdlColor);
	//		}
	//		return nullptr;
	//	};

	//TTF_SetFontOutline(m_font, 0);
	//SDL_Surface* textSurface = RenderPass(m_color);
	//if (textSurface == nullptr)
	//{
	//	SDL_Log("Lib/Debug.cpp/Update/RenderPass/textSurface non reussie : %s", SDL_GetError());
	//	return;
	//}
	//SDL_Surface* convertedSurface = nullptr;
	//if (m_outlineSize <= 0)
	//{
	//	convertedSurface = SDL_ConvertSurface(textSurface, SDL_PIXELFORMAT_RGBA32);
	//	SDL_DestroySurface(textSurface);
	//	if (convertedSurface == nullptr)
	//	{
	//		SDL_Log("Lib/Debug.cpp/Update/SDL_ConvertSurface/convertedSurface non reussie : %s", SDL_GetError());
	//		return;
	//	}
	//}
	//else
	//{
	//	TTF_SetFontOutline(m_font, m_outlineSize);
	//	SDL_Surface* outlineSurface = RenderPass(m_outlineColor);
	//	TTF_SetFontOutline(m_font, 0);

	//	if (outlineSurface == nullptr)
	//	{
	//		SDL_Log("Lib/Debug.cpp/Update/RenderPass/outlineSurface non reussie : %s", SDL_GetError());
	//		SDL_DestroySurface(textSurface);
	//		return;
	//	}

	//	SDL_Surface* textRGBA = SDL_ConvertSurface(textSurface, SDL_PIXELFORMAT_RGBA32);
	//	if (textRGBA == nullptr)
	//	{
	//		SDL_Log("Lib/Debug.cpp/Update/SDL_ConvertSurface/textRGBA non reussie : %s", SDL_GetError());
	//		SDL_DestroySurface(textSurface);
	//		SDL_DestroySurface(outlineSurface);
	//		return;
	//	}
	//	SDL_Surface* outlineRGBA = SDL_ConvertSurface(outlineSurface, SDL_PIXELFORMAT_RGBA32);
	//	if (outlineRGBA == nullptr)
	//	{
	//		SDL_Log("Lib/Debug.cpp/Update/SDL_ConvertSurface/outlineRGBA non reussie : %s", SDL_GetError());
	//		SDL_DestroySurface(textSurface);
	//		SDL_DestroySurface(outlineSurface);
	//		SDL_DestroySurface(textRGBA);
	//		return;
	//	}
	//	SDL_DestroySurface(textSurface);
	//	SDL_DestroySurface(outlineSurface);

	//	if (SDL_SetSurfaceBlendMode(textRGBA, SDL_BLENDMODE_BLEND) == false)
	//	{
	//		SDL_Log("Lib/Debug.cpp/Update/SDL_SetSurfaceBlendMode non reussie : %s", SDL_GetError());
	//		SDL_DestroySurface(textRGBA);
	//		SDL_DestroySurface(outlineRGBA);
	//		return;
	//	}

	//	SDL_Rect dstRect{ m_outlineSize, m_outlineSize, textRGBA->w, textRGBA->h };
	//	if (SDL_BlitSurface(textRGBA, nullptr, outlineRGBA, &dstRect) == false)
	//	{
	//		SDL_Log("Lib/Debug.cpp/Update/SDL_BlitSurface non reussie : %s", SDL_GetError());
	//		SDL_DestroySurface(textRGBA);
	//		SDL_DestroySurface(outlineRGBA);
	//		return;
	//	}

	//	convertedSurface = outlineRGBA;
	//	SDL_DestroySurface(textRGBA);
	//	//SDL_DestroySurface(outlineRGBA);
	//}

	//float currentW, currentH;
	//SDL_GetTextureSize(m_textTexture, &currentW, &currentH);
	//if ((int)currentW != convertedSurface->w || (int)currentH != convertedSurface->h)
	//{
	//	CreateTexture({ convertedSurface->w, convertedSurface->h });
	//	if (m_textTexture == nullptr)
	//	{
	//		SDL_DestroySurface(convertedSurface);
	//		return;
	//	}
	//}

	////SDL_PropertiesID props = SDL_GetTextureProperties(m_textTexture);
	////if (props == 0)
	////{
	////	SDL_Log("Lib/Debug.cpp/Update/SDL_GetTextureProperties/SDL_GetTextureProperties non reussie : %s", SDL_GetError());
	////	return;
	////}

	////int format = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, 0);
	////int access = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_ACCESS_NUMBER, 0);
	////int width = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
	////int height = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

	////std::vector<Uint32> pixels(width * height, 0); // Crée un vecteur de pixels initialisé à zéro (transparent)
	////if (SDL_UpdateTexture(m_textTexture, NULL, pixels.data(), width * sizeof(Uint32)) == false)
	////{
	////	SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
	////	return;
	////}

	//if (SDL_UpdateTexture(m_textTexture, NULL, convertedSurface->pixels, convertedSurface->pitch) == false)
	//{
	//	SDL_Log("Lib/Text.cpp/Update/SDL_UpdateTexture/cela na pas fonctionn� : %s", SDL_GetError());
	//	SDL_DestroySurface(convertedSurface);
	//	return;
	//}
	//SDL_DestroySurface(convertedSurface);

	//float textWidth, textHeight;
	//if (SDL_GetTextureSize(m_textTexture, &textWidth, &textHeight) == false)
	//{
	//	SDL_Log("Lib/Debug.cpp/Update/SDL_QueryTexture/SDL_QueryTexture non reussie : %s", SDL_GetError());
	//	return;
	//}
	//SetDimension({ (int)textWidth, (int)textHeight });
	////std::cerr << "Taille texture pour le text : " << textSizeW << " " << textSizeH << std::endl;
}

void Text::SetPosition(const Vector2f& p_position, Vector2f p_anchor)
{
	Transformable::SetPosition(p_position, p_anchor);
	m_positionWanted = p_position;
}

void Text::SetColor(Color p_color)
{
	Geometry::SetColor(p_color);

	if (TTF_SetTextColor(m_dynamicText, p_color.r, p_color.g, p_color.b, p_color.a) == false)
	{
		SDL_Log("Text::SetColor/TTF_SetTextColor failed : %s", SDL_GetError());
		return;
	}
}

Color Text::GetColor() const
{
	Color color;
	if (TTF_GetTextColor(m_dynamicText, &color.r, &color.g, &color.b, &color.a) == false)
	{
		SDL_Log("Text::GetColor/TTF_GetTextColor m_dynamicText failed : %s", SDL_GetError());
		return { 0, 0, 0, 0 };
	}
	return color;
}

void Text::SetWrapWidth(int p_wrapWidth)
{
	if (p_wrapWidth < 0)
	{
		SDL_Log("Lib/Text.cpp/SetWrapWidth/La largeur de retour � la ligne ne peut pas etre negative");
		return;
	}

	if (TTF_SetTextWrapWidth(m_dynamicText, p_wrapWidth) == false)
	{
		SDL_Log("Text::SetWrapWidth/TTF_SetTextWrapWidth m_dynamicText failed : %s", SDL_GetError());
		return;
	}
	if (TTF_SetTextWrapWidth(m_outlineText, p_wrapWidth) == false)
	{
		SDL_Log("Text::SetWrapWidth/TTF_SetTextWrapWidth m_outlineText failed : %s", SDL_GetError());
		return;
	}

	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

int Text::GetWrapWidth() const
{
	int wrapWidth;

	if (TTF_GetTextWrapWidth(m_dynamicText, &wrapWidth) == false)
	{
		SDL_Log("Text::GetWrapWidth/TTF_GetTextWrapWidth m_dynamicText failed : %s", SDL_GetError());
		return 0;
	}

	return wrapWidth;
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
	TTF_SetFontStyle(m_outlineFont, m_fontStyle);

	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

FontStyle Text::GetFontStyle() const
{
	TTF_FontStyleFlags style = TTF_GetFontStyle(m_font);
	return (FontStyle)style;
}

void Text::SetOutlineSize(int p_outlineSize)
{
	if (p_outlineSize <= 0)
	{
		m_isHasOutline = false;
		m_outlineSize = 0;
		if (m_outlineText)
		{
			if (TTF_SetFontOutline(m_outlineFont, 0) == false)
			{
				SDL_Log("Text::SetOutlineSize/TTF_SetFontOutline failed : %s", SDL_GetError());
				return;
			}
		}
		SDL_Log("Lib/Text.cpp/SetOutlineSize/La taille du contour ne peut pas etre negative");
	}
	else if (p_outlineSize > 0)
	{
		m_isHasOutline = true;
		m_outlineSize = p_outlineSize;

		if (TTF_SetFontOutline(m_outlineFont, m_outlineSize) == false)
		{
			SDL_Log("Text::SetOutlineSize/TTF_SetFontOutline failed : %s", SDL_GetError());
			return;
		}
	}
	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

void Text::SetOutLineColor(Color p_color)
{
	if (TTF_SetTextColor(m_outlineText, p_color.r, p_color.g, p_color.b, p_color.a) == false)
	{
		SDL_Log("Text::SetColor/TTF_SetTextColor failed : %s", SDL_GetError());
		return;
	}
}

Color Text::GetOutLineColor() const
{
	Color color;
	if (TTF_GetTextColor(m_outlineText, &color.r, &color.g, &color.b, &color.a) == false)
	{
		SDL_Log("Text::GetColor/TTF_GetTextColor m_outlineText failed : %s", SDL_GetError());
		return { 0, 0, 0, 0 };
	}
	return color;
}

void Text::SetLineSpacing(int p_lineSpacing)
{
	TTF_SetFontLineSkip(m_font, p_lineSpacing);
	TTF_SetFontLineSkip(m_outlineFont, p_lineSpacing);

	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

int Text::GetLineSpacing() const
{
	int spacing = TTF_GetFontLineSkip(m_font);
	return spacing;
}

void Text::HorizontalAlignText(HorizontalAlignment p_alignment)
{
	TTF_SetFontWrapAlignment(m_font, (TTF_HorizontalAlignment)p_alignment);
	TTF_SetFontWrapAlignment(m_outlineFont, (TTF_HorizontalAlignment)p_alignment);

	SetPosition(m_positionWanted);

	SetDimension(GetTextSize());
}

HorizontalAlignment Text::GetHorizontalAlignText() const
{
	HorizontalAlignment alignment = (HorizontalAlignment)TTF_GetFontWrapAlignment(m_font);
	return alignment;
}

Vector2i Text::GetTextSize() const
{
	int width, height;
	if (TTF_GetTextSize(m_outlineText, &width, &height) == false)
	{
		SDL_Log("Text::GetTextSize/TTF_GetTextSize failed : %s", SDL_GetError());
		return { 0, 0 };
	}
	return { width, height };
}

void Text::CutTextBySize(Vector2i p_maxSize)
{
	Vector2i textSize = GetTextSize();
	while (textSize.GetX() > p_maxSize.GetX() || textSize.GetY() > p_maxSize.GetY())
	{
		if (m_text.empty())
		{
			break;
		}
		m_text.pop_back();
		SetText(m_text);
		textSize = GetTextSize();
	}
}
