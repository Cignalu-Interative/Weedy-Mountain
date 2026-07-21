#include "pch.h"
#include "Debug.h"
#include "Render.h"
#include "Window.h"
#include "Utils.h"

Debug* Debug::Get()
{
	static Debug instance;
	return &instance;
}

Debug::Debug()
{
	m_points.reserve(32);
	m_pointsVector.reserve(32);

	m_lines.reserve(32);
	m_linesVector.reserve(32);

	m_rects.reserve(32);
	m_rectsVector.reserve(32);

	m_fillRects.reserve(32);
	m_fillRectsVector.reserve(32);

	m_circles.reserve(32);
	m_circlesVector.reserve(32);

	m_fillCircles.reserve(32);
	m_fillCirclesVector.reserve(32);

	m_texts.reserve(32);
	m_textsVector.reserve(32);

	m_autoPointsCache.reserve(32);
	m_autoLinesCache.reserve(32);
	m_autoRectsCache.reserve(32);
	m_autoFillRectsCache.reserve(32);
	m_autoCirclesCache.reserve(32);
	m_autoFillCirclesCache.reserve(32);
	m_autoTextsCache.reserve(32);

	m_textEnginePropertiesID = SDL_CreateProperties();
}

Debug::~Debug()
{
	m_points.clear();
	m_lines.clear();
	m_rects.clear();
	m_fillRects.clear();
	m_circles.clear();
	m_fillCircles.clear();
	m_texts.clear();
	if (m_textEnginePropertiesID)
	{
		SDL_DestroyProperties(m_textEnginePropertiesID);
	}
	if (m_textEngine)
	{
		TTF_DestroyRendererTextEngine(m_textEngine);
	}
}

void Debug::SetRenderer(SDL_Renderer* p_renderer)
{
	m_renderer = p_renderer;
}

void Debug::CreateTextEngineProperties()
{
	if (m_renderer == nullptr)
	{
		SDL_Log("Debug::CreateTextEngineProperties: Renderer is null, cannot create text engine properties.");
		return;
	}

	SDL_SetPointerProperty(m_textEnginePropertiesID, "SDL_ttf.renderer_text_engine.create.renderer", m_renderer);
	SDL_SetNumberProperty(m_textEnginePropertiesID, "SDL_ttf.renderer_text_engine.create.atlas_texture_size", 2048);

	m_textEngine = TTF_CreateRendererTextEngineWithProperties(m_textEnginePropertiesID);
	if (m_textEngine == NULL)
	{
		SDL_Log("Debug::CreateTextEngineProperties: Failed to create text engine: %s", SDL_GetError());
		return;
	}
}

void Debug::SetDeltaTimeCallback(DeltaTimeCallback p_callback)
{
	m_deltaTimeCallback = std::move(p_callback);
}

float Debug::GetDeltaTime()
{
	if (m_deltaTimeCallback)
	{
		return m_deltaTimeCallback();
	}

	return 0.0f;
}

Point* Debug::CreatePoint(std::string p_id, Vector2f p_position, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_points.find(p_id);
	if (it != m_points.end())
	{
		Point* point = m_pointsVector[it->second];
		point->SetPosition(p_position);
		point->SetColor(p_color);
		point->SetIsGUI(p_isGUI);
		point->SetIsClamp(p_clampToScreen);
		return point;
	}

	Point* point = new Point(p_position, p_color);
	point->SetType((int)Type::Point);
	point->SetIsGUI(p_isGUI);
	point->SetIsClamp(p_clampToScreen);

	if (p_id.empty())
	{
		p_id = "point_auto_" + std::to_string(m_pointCount++);
	}
	point->SetId(p_id);
	m_points[std::move(p_id)] = m_pointsVector.size();
	m_pointsVector.push_back(point);

	SortGeometriesByDrawLayer();
	return point;
}
Line* Debug::CreateLine(std::string p_id, Vector2f p_start, Vector2f p_end, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_lines.find(p_id);
	if (it != m_lines.end())
	{
		Line* line = m_linesVector[it->second];
		line->SetPosition(p_start);
		line->SetEndPosition(p_end);
		line->SetColor(p_color);
		line->SetIsGUI(p_isGUI);
		line->SetIsClamp(p_clampToScreen);
		return line;
	}

	Line* line = new Line(p_start, p_end, p_color);
	line->SetType((int)Type::Line);
	line->SetIsGUI(p_isGUI);
	line->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "line_auto_" + std::to_string(m_lineCount++);
	}
	line->SetId(p_id);
	m_lines[std::move(p_id)] = m_linesVector.size();
	m_linesVector.push_back(line);

	SortGeometriesByDrawLayer();
	return line;
}
Rectangle* Debug::CreateRect(std::string p_id, Vector2f p_position, Vector2i p_dim, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_rects.find(p_id);
	if (it != m_rects.end())
	{
		Rectangle* rect = m_rectsVector[it->second];
		rect->SetDimension(p_dim);
		rect->SetPosition(p_position);
		rect->SetColor(p_color);
		rect->SetIsGUI(p_isGUI);
		rect->SetIsClamp(p_clampToScreen);
		return rect;
	}

	Rectangle* rect = new Rectangle;
	rect->Initialize(p_position, p_dim, p_color);
	rect->SetType((int)Type::Rect);
	rect->SetIsGUI(p_isGUI);
	rect->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "rect_auto_" + std::to_string(m_rectCount++);
	}
	rect->SetId(p_id);
	m_rects[std::move(p_id)] = m_rectsVector.size();
	m_rectsVector.push_back(rect);

	SortGeometriesByDrawLayer();
	return rect;
}
Rectangle* Debug::CreateFillRect(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_fillRects.find(p_id);
	if (it != m_fillRects.end())
	{
		Rectangle* rect = m_fillRectsVector[it->second];
		rect->SetDimension(p_dimension);
		rect->SetPosition(p_position);
		rect->SetColor(p_color);
		rect->SetIsGUI(p_isGUI);
		rect->SetIsClamp(p_clampToScreen);
		return rect;
	}

	Rectangle* rect = new Rectangle;
	rect->Initialize(p_position, p_dimension, p_color);
	rect->SetType((int)Type::FillRect);
	rect->SetFilled(true);
	rect->SetIsGUI(p_isGUI);
	rect->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "fillRect_auto_" + std::to_string(m_fillRectCount++);
	}
	rect->SetId(p_id);
	m_fillRects[std::move(p_id)] = m_fillRectsVector.size();
	m_fillRectsVector.push_back(rect);

	SortGeometriesByDrawLayer();
	return rect;
}
Circle* Debug::CreateCircle(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_circles.find(p_id);
	if (it != m_circles.end())
	{
		Circle* circle = m_circlesVector[it->second];
		circle->SetDimension(p_dimension);
		circle->SetPosition(p_position);
		circle->SetColor(p_color);
		circle->SetIsGUI(p_isGUI);
		circle->SetIsClamp(p_clampToScreen);
		return circle;
	}

	Circle* circle = new Circle;
	circle->Initialize(p_position, p_dimension, p_color);
	circle->SetType((int)Type::Circle);
	circle->SetIsGUI(p_isGUI);
	circle->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "circle_auto_" + std::to_string(m_circleCount++);
	}
	circle->SetId(p_id);
	m_circles[std::move(p_id)] = m_circlesVector.size();
	m_circlesVector.push_back(circle);

	SortGeometriesByDrawLayer();
	return circle;
}
Circle* Debug::CreateFillCircle(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_fillCircles.find(p_id);
	if (it != m_fillCircles.end())
	{
		Circle* circle = m_fillCirclesVector[it->second];
		circle->SetDimension(p_dimension);
		circle->SetPosition(p_position);
		circle->SetColor(p_color);
		circle->SetIsGUI(p_isGUI);
		circle->SetIsClamp(p_clampToScreen);
		return circle;
	}

	Circle* circle = new Circle;
	circle->Initialize(p_position, p_dimension, p_color);
	circle->SetType((int)Type::FillCircle);
	circle->SetFilled(true);
	circle->SetIsGUI(p_isGUI);
	circle->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "fillCircle_auto_" + std::to_string(m_fillCircleCount++);
	}
	circle->SetId(p_id);
	m_fillCircles[std::move(p_id)] = m_fillCirclesVector.size();
	m_fillCirclesVector.push_back(circle);

	SortGeometriesByDrawLayer();
	return circle;
}
Text* Debug::CreateText(std::string p_id, Vector2f p_position, const std::string& p_text, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_texts.find(p_id);
	if (it != m_texts.end())
	{
		Text* text = m_textsVector[it->second];
		text->SetText(p_text);
		text->SetPosition(p_position);
		text->SetColor(p_color);
		text->SetIsGUI(p_isGUI);
		text->SetIsClamp(p_clampToScreen);
		return text;
	}

	Text* text = new Text();
	text->SetRenderer(m_renderer);
	text->SetTextEngine(m_textEngine);
	text->Initialize(p_position, p_text, p_color);
	text->SetType((int)Type::Text);
	text->SetIsGUI(p_isGUI);
	text->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "text_auto_" + std::to_string(m_textCount++);
	}
	text->SetId(p_id);
	m_texts[std::move(p_id)] = m_textsVector.size();
	m_textsVector.push_back(text);

	SortGeometriesByDrawLayer();
	return text;
}

Sprite* Debug::CreateSprite(std::string p_id, Vector2f p_position, Vector2i p_dimension, bool p_isGUI = false, bool p_clampToScreen = true)
{
	auto it = m_sprites.find(p_id);
	if (it != m_sprites.end())
	{
		Sprite* sprite = m_spritesVector[it->second];
		sprite->SetPosition(p_position);
		sprite->SetIsGUI(p_isGUI);
		sprite->SetIsClamp(p_clampToScreen);
		return sprite;
	}

	Sprite* sprite = new Sprite;
	sprite->SetType((int)Type::Sprite);
	sprite->SetIsGUI(p_isGUI);
	sprite->SetIsClamp(p_clampToScreen);

	if (p_id.empty())
	{
		p_id = "sprite_auto_" + std::to_string(m_spriteCount++);
	}
	sprite->SetId(p_id);
	m_sprites[std::move(p_id)] = m_spritesVector.size();
	m_spritesVector.push_back(sprite);

	SortGeometriesByDrawLayer();
	return sprite;
}

void Debug::destroyPoint(const std::string& p_id)
{
	if (m_pointsVector.empty())
	{
		return;
	}
	auto it = m_points.find(p_id);

	if (it == m_points.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_pointsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_pointsVector[indexToDelete], m_pointsVector[lastIndex]);
		m_points[m_pointsVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_pointsVector[lastIndex];
	m_pointsVector[lastIndex] = nullptr;
	m_pointsVector.pop_back();
	m_points.erase(it);
}
void Debug::destroyLine(const std::string& p_id)
{
	if (m_linesVector.empty())
	{
		return;
	}
	auto it = m_lines.find(p_id);

	if (it == m_lines.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_linesVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_linesVector[indexToDelete], m_linesVector[lastIndex]);
		m_lines[m_linesVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_linesVector[lastIndex];
	m_linesVector[lastIndex] = nullptr;
	m_linesVector.pop_back();
	m_lines.erase(it);
}
void Debug::destroyRect(const std::string& p_id)
{
	if (m_rectsVector.empty())
	{
		return;
	}
	auto it = m_rects.find(p_id);

	if (it == m_rects.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_rectsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_rectsVector[indexToDelete], m_rectsVector[lastIndex]);
		m_rects[m_rectsVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_rectsVector[lastIndex];
	m_rectsVector[lastIndex] = nullptr;
	m_rectsVector.pop_back();
	m_rects.erase(it);
}
void Debug::destroyFillRect(const std::string& p_id)
{
	if (m_fillRectsVector.empty())
	{
		return;
	}
	auto it = m_fillRects.find(p_id);

	if (it == m_fillRects.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_fillRectsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_fillRectsVector[indexToDelete], m_fillRectsVector[lastIndex]);
		m_fillRects[m_fillRectsVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_fillRectsVector[lastIndex];
	m_fillRectsVector[lastIndex] = nullptr;
	m_fillRectsVector.pop_back();
	m_fillRects.erase(it);
}
void Debug::destroyCircle(const std::string& p_id)
{
	if (m_circlesVector.empty())
	{
		return;
	}
	auto it = m_circles.find(p_id);

	if (it == m_circles.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_circlesVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_circlesVector[indexToDelete], m_circlesVector[lastIndex]);
		m_circles[m_circlesVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_circlesVector[lastIndex];
	m_circlesVector[lastIndex] = nullptr;
	m_circlesVector.pop_back();
	m_circles.erase(it);
}
void Debug::destroyFillCircle(const std::string& p_id)
{
	if (m_fillCirclesVector.empty())
	{
		return;
	}
	auto it = m_fillCircles.find(p_id);

	if (it == m_fillCircles.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_fillCirclesVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_fillCirclesVector[indexToDelete], m_fillCirclesVector[lastIndex]);
		m_fillCircles[m_fillCirclesVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_fillCirclesVector[lastIndex];
	m_fillCirclesVector[lastIndex] = nullptr;
	m_fillCirclesVector.pop_back();
	m_fillCircles.erase(it);
}
void Debug::destroyText(const std::string& p_id)
{
	if (m_textsVector.empty())
	{
		return;
	}
	auto it = m_texts.find(p_id);

	if (it == m_texts.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_textsVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_textsVector[indexToDelete], m_textsVector[lastIndex]);
		m_texts[m_textsVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_textsVector[lastIndex];
	m_textsVector[lastIndex] = nullptr;
	m_textsVector.pop_back();
	m_texts.erase(it);
}

void Debug::destroySprite(const std::string& p_id)
{
	if (m_spritesVector.empty())
	{
		return;
	}
	auto it = m_sprites.find(p_id);

	if (it == m_sprites.end())
	{
		return;
	}

	size_t indexToDelete = it->second;
	size_t lastIndex = m_spritesVector.size() - 1;

	if (indexToDelete != lastIndex)
	{
		std::swap(m_spritesVector[indexToDelete], m_spritesVector[lastIndex]);
		m_sprites[m_spritesVector[indexToDelete]->GetId()] = indexToDelete;
	}
	delete m_spritesVector[lastIndex];
	m_spritesVector[lastIndex] = nullptr;
	m_spritesVector.pop_back();
	m_sprites.erase(it);
}

void Debug::Destroy(const std::string& p_id)
{
	destroyPoint(p_id);
	destroyLine(p_id);
	destroyRect(p_id);
	destroyFillRect(p_id);
	destroyCircle(p_id);
	destroyFillCircle(p_id);
	destroyText(p_id);
	destroySprite(p_id);
}

void Debug::ClearPoints()
{
	for (auto& point : m_pointsVector)
	{
		delete point;
	}
	m_pointsVector.clear();
	m_points.clear();
}
void Debug::ClearLines()
{
	for (auto& line : m_linesVector)
	{
		delete line;
	}
	m_linesVector.clear();
	m_lines.clear();
}
void Debug::ClearRects()
{
	for (auto& rect : m_rectsVector)
	{
		delete rect;
	}
	m_rectsVector.clear();
	m_rects.clear();
}
void Debug::ClearFillRects()
{
	for (auto& fillRect : m_fillRectsVector)
	{
		delete fillRect;
	}
	m_fillRectsVector.clear();
	m_fillRects.clear();
}
void Debug::ClearCircles()
{
	for (auto& circle : m_circlesVector)
	{
		delete circle;
	}
	m_circlesVector.clear();
	m_circles.clear();
}
void Debug::ClearFillCircles()
{
	for (auto& fillCircle : m_fillCirclesVector)
	{
		delete fillCircle;
	}
	m_fillCirclesVector.clear();
	m_fillCircles.clear();
}
void Debug::ClearTexts()
{
	for (auto& text : m_textsVector)
	{
		delete text;
	}
	m_textsVector.clear();
	m_texts.clear();
}

void Debug::ClearSprites()
{
	for (auto& sprite : m_spritesVector)
	{
		delete sprite;
	}
	m_spritesVector.clear();
	m_sprites.clear();
}

void Debug::ClearAll()
{
	ClearPoints();
	ClearLines();
	ClearRects();
	ClearFillRects();
	ClearCircles();
	ClearFillCircles();
	ClearTexts();
	ClearSprites();
}

Point* Debug::GetPoint(const std::string& p_id)
{
	auto it = m_points.find(p_id);
	if (it != m_points.end())
	{
		return m_pointsVector[it->second];
	}
	//else
	//{
	//	CreatePoint(p_id, { 0, 0 }, { 255, 255, 255, 255 });
	//	it = m_points.find(p_id);
	//	return m_pointsVector[it->second];
	//}

	return nullptr;
}
Line* Debug::GetLine(const std::string& p_id)
{
	auto it = m_lines.find(p_id);
	if (it != m_lines.end())
	{
		return m_linesVector[it->second];
	}
	//else
	//{
	//	CreateLine(p_id, { 0, 0 }, { 100, 100 }, { 255, 255, 255, 255 });
	//	it = m_lines.find(p_id);
	//	return m_linesVector[it->second];
	//}

	return nullptr;
}
Rectangle* Debug::GetRect(const std::string& p_id)
{
	auto it = m_rects.find(p_id);
	if (it != m_rects.end())
	{
		return m_rectsVector[it->second];
	}
	//else
	//{
	//	CreateRect(p_id, { 0, 0 }, { 100, 100 }, { 255, 255, 255, 255 });
	//	it = m_rects.find(p_id);
	//	return m_rectsVector[it->second];
	//}

	return nullptr;
}
Rectangle* Debug::GetFillRect(const std::string& p_id)
{
	auto it = m_fillRects.find(p_id);
	if (it != m_fillRects.end())
	{
		return m_fillRectsVector[it->second];
	}
	//else
	//{
	//	CreateFillRect(p_id, { 0, 0 }, { 100, 100 }, { 255, 255, 255, 255 });
	//	it = m_fillRects.find(p_id);
	//	return m_fillRectsVector[it->second];
	//}

	return nullptr;
}
Circle* Debug::GetCircle(const std::string& p_id)
{
	auto it = m_circles.find(p_id);
	if (it != m_circles.end())
	{
		return m_circlesVector[it->second];
	}
	//else
	//{
	//	CreateCircle(p_id, { 0, 0 }, {50, 50}, {255, 255, 255, 255});
	//	it = m_circles.find(p_id);
	//	return m_circlesVector[it->second];
	//}

	return nullptr;
}
Circle* Debug::GetFillCircle(const std::string& p_id)
{
	auto it = m_fillCircles.find(p_id);
	if (it != m_fillCircles.end())
	{
		return m_fillCirclesVector[it->second];
	}
	//else
	//{
	//	CreateFillCircle(p_id, { 0, 0 }, {50, 50}, { 255, 255, 255, 255 });
	//	it = m_fillCircles.find(p_id);
	//	return m_fillCirclesVector[it->second];
	//}

	return nullptr;
}
Text* Debug::GetText(const std::string& p_id)
{
	auto it = m_texts.find(p_id);
	if (it != m_texts.end())
	{
		return m_textsVector[it->second];
	}
	//else
	//{
	//	CreateText(p_id, { 0, 0 }, p_id, { 255, 255, 255, 255 });
	//	it = m_texts.find(p_id);
	//	return m_textsVector[it->second];
	//}

	return nullptr;
}

Sprite* Debug::GetSprite(const std::string& p_id)
{
	auto it = m_sprites.find(p_id);
	if (it != m_sprites.end())
	{
		return m_spritesVector[it->second];
	}

	return nullptr;
}

std::vector<Point*> Debug::GetAllAutoPoints() const
{
	m_autoPointsCache.clear();
	for (Point* point : m_pointsVector)
	{
		if (point->GetId().find("point_auto_") != std::string::npos)
		{
			m_autoPointsCache.push_back(point);
		}
	}
	return m_autoPointsCache;
}

std::vector<Line*> Debug::GetAllAutoLines() const
{
	m_autoLinesCache.clear();
	for (Line* line : m_linesVector)
	{
		if (line->GetId().find("line_auto_") != std::string::npos)
		{
			m_autoLinesCache.push_back(line);
		}
	}
	return m_autoLinesCache;
}

std::vector<Rectangle*> Debug::GetAllAutoRects() const
{
	m_autoRectsCache.clear();
	for (Rectangle* rect : m_rectsVector)
	{
		if (rect->GetId().find("rect_auto_") != std::string::npos)
		{
			m_autoRectsCache.push_back(rect);
		}
	}
	return m_autoRectsCache;
}

std::vector<Rectangle*> Debug::GetAllAutoFillRects() const
{
	m_autoFillRectsCache.clear();
	for (Rectangle* rect : m_fillRectsVector)
	{
		if (rect->GetId().find("fillrect_auto_") != std::string::npos)
		{
			m_autoFillRectsCache.push_back(rect);
		}
	}
	return m_autoFillRectsCache;
}

std::vector<Circle*> Debug::GetAllAutoCircles() const
{
	m_autoCirclesCache.clear();
	for (Circle* circle : m_circlesVector)
	{
		if (circle->GetId().find("circle_auto_") != std::string::npos)
		{
			m_autoCirclesCache.push_back(circle);
		}
	}
	return m_autoCirclesCache;
}

std::vector<Circle*> Debug::GetAllAutoFillCircles() const
{
	m_autoFillCirclesCache.clear();
	for (Circle* circle : m_fillCirclesVector)
	{
		if (circle->GetId().find("fillcircle_auto_") != std::string::npos)
		{
			m_autoFillCirclesCache.push_back(circle);
		}
	}
	return m_autoFillCirclesCache;
}

std::vector<Text*>& Debug::GetAllAutoTexts() const
{
	m_autoTextsCache.clear();
	for (Text* text : m_textsVector)
	{
		if (text->GetId().find("text_auto_") != std::string::npos)
		{
			m_autoTextsCache.push_back(text);
		}
	}
	return m_autoTextsCache;
}

std::vector<Sprite*>& Debug::GetAllAutoSprites() const
{
	m_autoSpritesCache.clear();
	for (Sprite* sprite : m_spritesVector)
	{
		if (sprite->GetId().find("sprite_auto_") != std::string::npos)
		{
			m_autoSpritesCache.push_back(sprite);
		}
	}
	return m_autoSpritesCache;
}

std::vector<Point*>& Debug::GetAllPoints()
{
	return m_pointsVector;
}

std::vector<Line*>& Debug::GetAllLines()
{
	return m_linesVector;
}

std::vector<Rectangle*>& Debug::GetAllRects()
{
	return m_rectsVector;
}

std::vector<Rectangle*>& Debug::GetAllFillRects()
{
	return m_fillRectsVector;
}

std::vector<Circle*>& Debug::GetAllCircles()
{
	return m_circlesVector;
}

std::vector<Circle*>& Debug::GetAllFillCircles()
{
	return m_fillCirclesVector;
}

std::vector<Text*>& Debug::GetAllTexts()
{
	return m_textsVector;
}

std::vector<Sprite*>& Debug::GetAllSprites()
{
	return m_spritesVector;
}

void Debug::SetPointDrawLayer(const std::string& p_id, int p_layer)
{
	Point* point = GetPoint(p_id);
	if (point)
	{
		point->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetLineDrawLayer(const std::string& p_id, int p_layer)
{
	Line* line = GetLine(p_id);
	if (line)
	{
		line->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetRectDrawLayer(const std::string& p_id, int p_layer)
{
	Rectangle* rect = GetRect(p_id);
	if (rect)
	{
		rect->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetFillRectDrawLayer(const std::string& p_id, int p_layer)
{
	Rectangle* fillRect = GetFillRect(p_id);
	if (fillRect)
	{
		fillRect->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetCircleDrawLayer(const std::string& p_id, int p_layer)
{
	Circle* circle = GetCircle(p_id);
	if (circle)
	{
		circle->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetFillCircleDrawLayer(const std::string& p_id, int p_layer)
{
	Circle* fillCircle = GetFillCircle(p_id);
	if (fillCircle)
	{
		fillCircle->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetTextDrawLayer(const std::string& p_id, int p_layer)
{
	Text* text = GetText(p_id);
	if (text)
	{
		text->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SetSpriteDrawLayer(const std::string& p_id, int p_layer)
{
	Sprite* sprite = GetSprite(p_id);
	if (sprite)
	{
		sprite->SetDrawLayer(p_layer);
	}
	SortGeometriesByDrawLayer();
}

void Debug::SortGeometriesByDrawLayer()
{
	m_geometries.clear();

	for (auto point : m_pointsVector)
	{
		m_geometries.push_back(point);
	}
	for (auto line : m_linesVector)
	{
		m_geometries.push_back(line);
	}
	for (auto rect : m_rectsVector)
	{
		m_geometries.push_back(rect);
	}
	for (auto fillRect : m_fillRectsVector)
	{
		m_geometries.push_back(fillRect);
	}
	for (auto circle : m_circlesVector)
	{
		m_geometries.push_back(circle);
	}
	for (auto fillCircle : m_fillCirclesVector)
	{
		m_geometries.push_back(fillCircle);
	}
	for (auto text : m_textsVector)
	{
		m_geometries.push_back(text);
	}
	for (auto sprite : m_spritesVector)
	{
		m_geometries.push_back(sprite);
	}

	std::sort(m_geometries.begin(), m_geometries.end(), [](Geometry* a, Geometry* b) {
		return a->GetDrawLayer() < b->GetDrawLayer();
		});
}

void Debug::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	for (auto& g : m_geometries)
	{
		if (g->GetType() == 0)
		{
			DrawPoint(dynamic_cast<Point*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 1)
		{
			DrawLine(dynamic_cast<Line*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 2)
		{
			DrawRect(dynamic_cast<Rectangle*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 3)
		{
			DrawFillRect(dynamic_cast<Rectangle*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 4)
		{
			DrawCircle(dynamic_cast<Circle*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 5)
		{
			DrawFillCircle(dynamic_cast<Circle*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 6)
		{
			DrawText(dynamic_cast<Text*>(g), f_render, f_window, p_camera);
		}
		if (g->GetType() == 7)
		{
			DrawSprite(dynamic_cast<Sprite*>(g), f_render, f_window, p_camera);
		}
	}
	/*DrawPoints(f_render, f_window, p_camera);
	DrawLines(f_render, f_window, p_camera);
	DrawRects(f_render, f_window, p_camera);
	DrawFillRects(f_render, f_window, p_camera);
	DrawCircles(f_render, f_window, p_camera);
	DrawFillCircles(f_render, f_window, p_camera);
	DrawTexts(f_render, f_window, p_camera);*/
}

void Debug::DrawPoints(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& point : m_pointsVector)
	{
		if (point->GetIsClamp() == true)
		{
			Vector2f pointPos = point->GetPosition();
			if (pointPos.GetX() < 0)
			{
				point->SetPosition({ static_cast<float>(0 + m_margin), point->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (pointPos.GetX() > windowW)
			{
				point->SetPosition({ static_cast<float>(windowW - m_margin), point->GetPosition().GetY() }, { 1, 0.5f });
			}

			if (pointPos.GetY() < 0)
			{
				point->SetPosition({ point->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (pointPos.GetY() > windowH)
			{
				point->SetPosition({ point->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		point->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawLines(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& line : m_linesVector)
	{
		if (line->GetIsClamp() == true)
		{
			Vector2f lineStartPos = line->GetPosition();
			Vector2f lineEndPos = line->GetEndPosition();
			if (lineStartPos.GetX() < 0)
			{
				line->SetPosition({ static_cast<float>(0 + m_margin), line->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (lineStartPos.GetX() > windowW)
			{
				line->SetPosition({ static_cast<float>(windowW - m_margin), line->GetPosition().GetY() }, { 1, 0.5f });
			}

			if (lineStartPos.GetY() < 0)
			{
				line->SetPosition({ line->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (lineStartPos.GetY() > windowH)
			{
				line->SetPosition({ line->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}

			if (lineEndPos.GetX() < 0)
			{
				line->SetEndPosition({ static_cast<float>(0 + m_margin), line->GetEndPosition().GetY() }, { 0, 0.5f });
			}
			else if (lineEndPos.GetX() > windowW)
			{
				line->SetEndPosition({ static_cast<float>(windowW - m_margin), line->GetEndPosition().GetY() }, { 1, 0.5f });
			}

			if (lineEndPos.GetY() < 0)
			{
				line->SetEndPosition({ line->GetEndPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (lineEndPos.GetY() > windowH)
			{
				line->SetEndPosition({ line->GetEndPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		line->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawRects(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& rectangle : m_rectsVector)
	{
		if (rectangle->GetIsClamp() == true)
		{
			int width = rectangle->GetDimension().GetX();
			int height = rectangle->GetDimension().GetY();

			if (rectangle->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				rectangle->SetPosition({ static_cast<float>(0 + m_margin), rectangle->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (rectangle->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				rectangle->SetPosition({ static_cast<float>(windowW - m_margin), rectangle->GetPosition().GetY() }, { 1, 0.5f });
			}
			if (rectangle->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				rectangle->SetPosition({ rectangle->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (rectangle->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				rectangle->SetPosition({ rectangle->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		rectangle->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawFillRects(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& rectangle : m_fillRectsVector)
	{
		if (rectangle->GetIsClamp() == true)
		{
			int width = rectangle->GetDimension().GetX();
			int height = rectangle->GetDimension().GetY();

			if (rectangle->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				rectangle->SetPosition({ static_cast<float>(0 + m_margin), rectangle->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (rectangle->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				rectangle->SetPosition({ static_cast<float>(windowW - m_margin), rectangle->GetPosition().GetY() }, { 1, 0.5f });
			}
			if (rectangle->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				rectangle->SetPosition({ rectangle->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (rectangle->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				rectangle->SetPosition({ rectangle->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		rectangle->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawCircles(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& circle : m_circlesVector)
	{
		int width = circle->GetDimension().GetX();
		int height = circle->GetDimension().GetY();
		if (circle->GetIsClamp() == true)
		{
			if (circle->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				circle->SetPosition({ static_cast<float>(0 + m_margin), circle->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (circle->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				circle->SetPosition({ static_cast<float>(windowW - m_margin), circle->GetPosition().GetY() }, { 1, 0.5f });
			}
			if (circle->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				circle->SetPosition({ circle->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (circle->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				circle->SetPosition({ circle->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		circle->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawFillCircles(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& circle : m_fillCirclesVector)
	{
		int width = circle->GetDimension().GetX();
		int height = circle->GetDimension().GetY();
		if (circle->GetIsClamp() == true)
		{
			if (circle->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				circle->SetPosition({ static_cast<float>(0 + m_margin), circle->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (circle->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				circle->SetPosition({ static_cast<float>(windowW - m_margin), circle->GetPosition().GetY() }, { 1, 0.5f });
			}
			if (circle->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				circle->SetPosition({ circle->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (circle->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				circle->SetPosition({ circle->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		circle->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawTexts(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& text : m_textsVector)
	{
		if (text->GetIsClamp() == true)
		{
			int width = text->GetDimension().GetX();
			int height = text->GetDimension().GetY();

			if (text->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				text->SetPosition({ static_cast<float>(0 + m_margin), text->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (text->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				text->SetPosition({ static_cast<float>(windowW - m_margin), text->GetPosition().GetY() }, { 1, 0.5f });
			}
			if (text->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				text->SetPosition({ text->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (text->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				text->SetPosition({ text->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		text->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawSprites(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& sprite : m_spritesVector)
	{
		if (sprite->GetIsClamp() == true)
		{
			int width = sprite->GetDimension().GetX();
			int height = sprite->GetDimension().GetY();

			if (sprite->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				sprite->SetPosition({ static_cast<float>(0 + m_margin), sprite->GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (sprite->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				sprite->SetPosition({ static_cast<float>(windowW - m_margin), sprite->GetPosition().GetY() }, { 1, 0.5f });
			}
			if (sprite->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				sprite->SetPosition({ sprite->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (sprite->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				sprite->SetPosition({ sprite->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		sprite->Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawPoint(Point* p_point, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	if (p_point->GetIsClamp() == true)
	{
		Vector2f pointPos = p_point->GetPosition();
		if (pointPos.GetX() < 0)
		{
			p_point->SetPosition({ static_cast<float>(0 + m_margin), p_point->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (pointPos.GetX() > windowW)
		{
			p_point->SetPosition({ static_cast<float>(windowW - m_margin), p_point->GetPosition().GetY() }, { 1, 0.5f });
		}

		if (pointPos.GetY() < 0)
		{
			p_point->SetPosition({ p_point->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (pointPos.GetY() > windowH)
		{
			p_point->SetPosition({ p_point->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_point->Draw(f_render, f_window, p_camera);
	
}

void Debug::DrawLine(Line* p_line, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	if (p_line->GetIsClamp() == true)
	{
		Vector2f lineStartPos = p_line->GetPosition();
		Vector2f lineEndPos = p_line->GetEndPosition();
		if (lineStartPos.GetX() < 0)
		{
			p_line->SetPosition({ static_cast<float>(0 + m_margin), p_line->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (lineStartPos.GetX() > windowW)
		{
			p_line->SetPosition({ static_cast<float>(windowW - m_margin), p_line->GetPosition().GetY() }, { 1, 0.5f });
		}

		if (lineStartPos.GetY() < 0)
		{
			p_line->SetPosition({ p_line->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (lineStartPos.GetY() > windowH)
		{
			p_line->SetPosition({ p_line->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}

		if (lineEndPos.GetX() < 0)
		{
			p_line->SetEndPosition({ static_cast<float>(0 + m_margin), p_line->GetEndPosition().GetY() }, { 0, 0.5f });
		}
		else if (lineEndPos.GetX() > windowW)
		{
			p_line->SetEndPosition({ static_cast<float>(windowW - m_margin), p_line->GetEndPosition().GetY() }, { 1, 0.5f });
		}

		if (lineEndPos.GetY() < 0)
		{
			p_line->SetEndPosition({ p_line->GetEndPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (lineEndPos.GetY() > windowH)
		{
			p_line->SetEndPosition({ p_line->GetEndPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_line->Draw(f_render, f_window, p_camera);
	
}

void Debug::DrawRect(Rectangle* p_rect, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	if (p_rect->GetIsClamp() == true)
	{
		int width = p_rect->GetDimension().GetX();
		int height = p_rect->GetDimension().GetY();

		if (p_rect->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
		{
			p_rect->SetPosition({ static_cast<float>(0 + m_margin), p_rect->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (p_rect->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
		{
			p_rect->SetPosition({ static_cast<float>(windowW - m_margin), p_rect->GetPosition().GetY() }, { 1, 0.5f });
		}
		if (p_rect->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
		{
			p_rect->SetPosition({ p_rect->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (p_rect->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
		{
			p_rect->SetPosition({ p_rect->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_rect->Draw(f_render, f_window, p_camera);
	
}

void Debug::DrawFillRect(Rectangle* p_rect, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	if (p_rect->GetIsClamp() == true)
	{
		int width = p_rect->GetDimension().GetX();
		int height = p_rect->GetDimension().GetY();

		if (p_rect->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
		{
			p_rect->SetPosition({ static_cast<float>(0 + m_margin), p_rect->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (p_rect->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
		{
			p_rect->SetPosition({ static_cast<float>(windowW - m_margin), p_rect->GetPosition().GetY() }, { 1, 0.5f });
		}
		if (p_rect->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
		{
			p_rect->SetPosition({ p_rect->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (p_rect->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
		{
			p_rect->SetPosition({ p_rect->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_rect->Draw(f_render, f_window, p_camera);
}

void Debug::DrawCircle(Circle* p_circle, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	int width = p_circle->GetDimension().GetX();
	int height = p_circle->GetDimension().GetY();
	if (p_circle->GetIsClamp() == true)
	{
		if (p_circle->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
		{
			p_circle->SetPosition({ static_cast<float>(0 + m_margin), p_circle->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (p_circle->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
		{
			p_circle->SetPosition({ static_cast<float>(windowW - m_margin), p_circle->GetPosition().GetY() }, { 1, 0.5f });
		}
		if (p_circle->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
		{
			p_circle->SetPosition({ p_circle->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (p_circle->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
		{
			p_circle->SetPosition({ p_circle->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_circle->Draw(f_render, f_window, p_camera);
}

void Debug::DrawFillCircle(Circle* p_circle, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	int width = p_circle->GetDimension().GetX();
	int height = p_circle->GetDimension().GetY();
	if (p_circle->GetIsClamp() == true)
	{
		if (p_circle->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
		{
			p_circle->SetPosition({ static_cast<float>(0 + m_margin), p_circle->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (p_circle->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
		{
			p_circle->SetPosition({ static_cast<float>(windowW - m_margin), p_circle->GetPosition().GetY() }, { 1, 0.5f });
		}
		if (p_circle->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
		{
			p_circle->SetPosition({ p_circle->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (p_circle->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
		{
			p_circle->SetPosition({ p_circle->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_circle->Draw(f_render, f_window, p_camera);
}

void Debug::DrawText(Text* p_text, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	
	if (p_text->GetIsClamp() == true)
	{
		int width = p_text->GetDimension().GetX();
		int height = p_text->GetDimension().GetY();

		if (p_text->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
		{
			p_text->SetPosition({ static_cast<float>(0 + m_margin), p_text->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (p_text->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
		{
			p_text->SetPosition({ static_cast<float>(windowW - m_margin), p_text->GetPosition().GetY() }, { 1, 0.5f });
		}
		if (p_text->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
		{
			p_text->SetPosition({ p_text->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (p_text->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
		{
			p_text->SetPosition({ p_text->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_text->Draw(f_render, f_window, p_camera);
}
void Debug::DrawSprite(Sprite* p_sprite, Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();

	if (p_sprite->GetIsClamp() == true)
	{
		int width = p_sprite->GetDimension().GetX();
		int height = p_sprite->GetDimension().GetY();

		if (p_sprite->GetPosition({ 0.0f, 0.5f }).GetX() < 0)
		{
			p_sprite->SetPosition({ static_cast<float>(0 + m_margin), p_sprite->GetPosition().GetY() }, { 0, 0.5f });
		}
		else if (p_sprite->GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
		{
			p_sprite->SetPosition({ static_cast<float>(windowW - m_margin), p_sprite->GetPosition().GetY() }, { 1, 0.5f });
		}
		if (p_sprite->GetPosition({ 0.5f, 0.0f }).GetY() < 0)
		{
			p_sprite->SetPosition({ p_sprite->GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
		}
		else if (p_sprite->GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
		{
			p_sprite->SetPosition({ p_sprite->GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
		}
	}
	p_sprite->Draw(f_render, f_window, p_camera);
}


