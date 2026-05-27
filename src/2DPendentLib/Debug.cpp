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

void Debug::CreatePoint(std::string p_id, Vector2f p_position, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_points.find(p_id);
	if (it != m_points.end())
	{
		Point* point = &m_pointsVector[it->second];
		point->SetPosition(p_position);
		point->SetColor(p_color);
		point->SetIsGUI(p_isGUI);
		point->SetIsClamp(p_clampToScreen);
		return;
	}

	Point point(p_position, p_color);
	point.SetIsGUI(p_isGUI);
	point.SetIsClamp(p_clampToScreen);

	if (p_id.empty())
	{
		p_id = "point_auto_" + std::to_string(m_pointCount++);
	}
	point.SetId(p_id);
	m_points[std::move(p_id)] = m_pointsVector.size();
	m_pointsVector.push_back(point);
}
void Debug::CreateLine(std::string p_id, Vector2f p_start, Vector2f p_end, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_lines.find(p_id);
	if (it != m_lines.end())
	{
		Line* line = &m_linesVector[it->second];
		line->SetPosition(p_start);
		line->SetEndPosition(p_end);
		line->SetColor(p_color);
		line->SetIsGUI(p_isGUI);
		line->SetIsClamp(p_clampToScreen);
		return;
	}

	Line line(p_start, p_end, p_color);
	line.SetIsGUI(p_isGUI);
	line.SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "line_auto_" + std::to_string(m_lineCount++);
	}
	line.SetId(p_id);
	m_lines[std::move(p_id)] = m_linesVector.size();
	m_linesVector.push_back(line);
}
void Debug::CreateRect(std::string p_id, Vector2f p_position, Vector2i p_dim, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_rects.find(p_id);
	if (it != m_rects.end())
	{
		Rectangle* rect = &m_rectsVector[it->second];
		rect->SetDimension(p_dim);
		rect->SetPosition(p_position);
		rect->SetColor(p_color);
		rect->SetIsGUI(p_isGUI);
		rect->SetIsClamp(p_clampToScreen);
		return;
	}

	Rectangle rect;
	rect.Initialize(p_position, p_dim, p_color);
	rect.SetIsGUI(p_isGUI);
	rect.SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "rect_auto_" + std::to_string(m_rectCount++);
	}
	rect.SetId(p_id);
	m_rects[std::move(p_id)] = m_rectsVector.size();
	m_rectsVector.push_back(rect);
}
void Debug::CreateFillRect(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_fillRects.find(p_id);
	if (it != m_fillRects.end())
	{
		Rectangle* rect = &m_fillRectsVector[it->second];
		rect->SetDimension(p_dimension);
		rect->SetPosition(p_position);
		rect->SetColor(p_color);
		rect->SetIsGUI(p_isGUI);
		rect->SetIsClamp(p_clampToScreen);
		return;
	}

	Rectangle rect;
	rect.Initialize(p_position, p_dimension, p_color);
	rect.SetFilled(true);
	rect.SetIsGUI(p_isGUI);
	rect.SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "fillRect_auto_" + std::to_string(m_fillRectCount++);
	}
	rect.SetId(p_id);
	m_fillRects[std::move(p_id)] = m_fillRectsVector.size();
	m_fillRectsVector.push_back(rect);
}
void Debug::CreateCircle(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_circles.find(p_id);
	if (it != m_circles.end())
	{
		Circle* circle = &m_circlesVector[it->second];
		circle->SetDimension(p_dimension);
		circle->SetPosition(p_position);
		circle->SetColor(p_color);
		circle->SetIsGUI(p_isGUI);
		circle->SetIsClamp(p_clampToScreen);
		return;
	}

	Circle circle;
	circle.Initialize(p_position, p_dimension, p_color);
	circle.SetIsGUI(p_isGUI);
	circle.SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "circle_auto_" + std::to_string(m_circleCount++);
	}
	circle.SetId(p_id);
	m_circles[std::move(p_id)] = m_circlesVector.size();
	m_circlesVector.push_back(circle);
}
void Debug::CreateFillCircle(std::string p_id, Vector2f p_position, Vector2i p_dimension, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_fillCircles.find(p_id);
	if (it != m_fillCircles.end())
	{
		Circle* circle = &m_fillCirclesVector[it->second];
		circle->SetDimension(p_dimension);
		circle->SetPosition(p_position);
		circle->SetColor(p_color);
		circle->SetIsGUI(p_isGUI);
		circle->SetIsClamp(p_clampToScreen);
		return;
	}

	Circle circle;
	circle.Initialize(p_position, p_dimension, p_color);
	circle.SetFilled(true);
	circle.SetIsGUI(p_isGUI);
	circle.SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "fillCircle_auto_" + std::to_string(m_fillCircleCount++);
	}
	circle.SetId(p_id);
	m_fillCircles[std::move(p_id)] = m_fillCirclesVector.size();
	m_fillCirclesVector.push_back(circle);
}
void Debug::CreateText(std::string p_id, Vector2f p_position, const std::string& p_text, Color p_color, bool p_isGUI, bool p_clampToScreen)
{
	auto it = m_texts.find(p_id);
	if (it != m_texts.end())
	{
		Text* text = m_textsVector[it->second];
		text->SetPosition(p_position);
		text->SetText(p_text);
		text->SetColor(p_color);
		text->SetIsGUI(p_isGUI);
		text->SetIsClamp(p_clampToScreen);
		return;
	}

	Text* text = new Text();
	text->SetRenderer(m_renderer);
	text->Initialize(p_position, p_text, p_color);
	text->SetIsGUI(p_isGUI);
	text->SetIsClamp(p_clampToScreen);
	if (p_id.empty())
	{
		p_id = "text_auto_" + std::to_string(m_textCount++);
	}
	text->SetId(p_id);
	m_texts[std::move(p_id)] = m_textsVector.size();
	m_textsVector.push_back(text);
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
		m_points[m_pointsVector[indexToDelete].GetId()] = indexToDelete;
	}

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
		m_lines[m_linesVector[indexToDelete].GetId()] = indexToDelete;
	}

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
		m_rects[m_rectsVector[indexToDelete].GetId()] = indexToDelete;
	}

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
		m_fillRects[m_fillRectsVector[indexToDelete].GetId()] = indexToDelete;
	}

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
		m_circles[m_circlesVector[indexToDelete].GetId()] = indexToDelete;
	}

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
		m_fillCircles[m_fillCirclesVector[indexToDelete].GetId()] = indexToDelete;
	}

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

void Debug::ClearPoints()
{
	m_points.clear();
	m_pointsVector.clear();
}
void Debug::ClearLines()
{
	m_linesVector.clear();
	m_lines.clear();
}
void Debug::ClearRects()
{
	m_rectsVector.clear();
	m_rects.clear();
}
void Debug::ClearFillRects()
{
	m_fillRectsVector.clear();
	m_fillRects.clear();
}
void Debug::ClearCircles()
{
	m_circlesVector.clear();
	m_circles.clear();
}
void Debug::ClearFillCircles()
{
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

void Debug::ClearAll()
{
	ClearPoints();
	ClearLines();
	ClearRects();
	ClearFillRects();
	ClearCircles();
	ClearFillCircles();
	ClearTexts();
}

Point* Debug::GetPoint(const std::string& p_id)
{
	auto it = m_points.find(p_id);
	if (it != m_points.end())
	{
		return &m_pointsVector[it->second];
	}
	else
	{
		CreatePoint(p_id, { 0, 0 }, { 255, 255, 255, 255 });
		it = m_points.find(p_id);
		return &m_pointsVector[it->second];
	}
}
Line* Debug::GetLine(const std::string& p_id)
{
	auto it = m_lines.find(p_id);
	if (it != m_lines.end())
	{
		return &m_linesVector[it->second];
	}
	else
	{
		CreateLine(p_id, { 0, 0 }, { 100, 100 }, { 255, 255, 255, 255 });
		it = m_lines.find(p_id);
		return &m_linesVector[it->second];
	}
}
Rectangle* Debug::GetRect(const std::string& p_id)
{
	auto it = m_rects.find(p_id);
	if (it != m_rects.end())
	{
		return &m_rectsVector[it->second];
	}
	else
	{
		CreateRect(p_id, { 0, 0 }, { 100, 100 }, { 255, 255, 255, 255 });
		it = m_rects.find(p_id);
		return &m_rectsVector[it->second];
	}
}
Rectangle* Debug::GetFillRect(const std::string& p_id)
{
	auto it = m_fillRects.find(p_id);
	if (it != m_fillRects.end())
	{
		return &m_fillRectsVector[it->second];
	}
	else
	{
		CreateFillRect(p_id, { 0, 0 }, { 100, 100 }, { 255, 255, 255, 255 });
		it = m_fillRects.find(p_id);
		return &m_fillRectsVector[it->second];
	}
}
Circle* Debug::GetCircle(const std::string& p_id)
{
	auto it = m_circles.find(p_id);
	if (it != m_circles.end())
	{
		return &m_circlesVector[it->second];
	}
	else
	{
		CreateCircle(p_id, { 0, 0 }, {50, 50}, {255, 255, 255, 255});
		it = m_circles.find(p_id);
		return &m_circlesVector[it->second];
	}
}
Circle* Debug::GetFillCircle(const std::string& p_id)
{
	auto it = m_fillCircles.find(p_id);
	if (it != m_fillCircles.end())
	{
		return &m_fillCirclesVector[it->second];
	}
	else
	{
		CreateFillCircle(p_id, { 0, 0 }, {50, 50}, { 255, 255, 255, 255 });
		it = m_fillCircles.find(p_id);
		return &m_fillCirclesVector[it->second];
	}
}
Text* Debug::GetText(const std::string& p_id)
{
	auto it = m_texts.find(p_id);
	if (it != m_texts.end())
	{
		return m_textsVector[it->second];
	}
	else
	{
		CreateText(p_id, { 0, 0 }, p_id, { 255, 255, 255, 255 });
		it = m_texts.find(p_id);
		return m_textsVector[it->second];
	}
}

std::vector<Point> Debug::GetAllAutoPoints() const
{
	m_autoPointsCache.clear();
	for (const Point& point : m_pointsVector)
	{
		if (point.GetId().find("point_auto_") != std::string::npos)
		{
			m_autoPointsCache.push_back(point);
		}
	}
	return m_autoPointsCache;
}

std::vector<Line> Debug::GetAllAutoLines() const
{
	m_autoLinesCache.clear();
	for (const Line& line : m_linesVector)
	{
		if (line.GetId().find("line_auto_") != std::string::npos)
		{
			m_autoLinesCache.push_back(line);
		}
	}
	return m_autoLinesCache;
}

std::vector<Rectangle> Debug::GetAllAutoRects() const
{
	m_autoRectsCache.clear();
	for (const Rectangle& rect : m_rectsVector)
	{
		if (rect.GetId().find("rect_auto_") != std::string::npos)
		{
			m_autoRectsCache.push_back(rect);
		}
	}
	return m_autoRectsCache;
}

std::vector<Rectangle> Debug::GetAllAutoFillRects() const
{
	m_autoFillRectsCache.clear();
	for (const Rectangle& rect : m_fillRectsVector)
	{
		if (rect.GetId().find("fillrect_auto_") != std::string::npos)
		{
			m_autoFillRectsCache.push_back(rect);
		}
	}
	return m_autoFillRectsCache;
}

std::vector<Circle> Debug::GetAllAutoCircles() const
{
	m_autoCirclesCache.clear();
	for (const Circle& circle : m_circlesVector)
	{
		if (circle.GetId().find("circle_auto_") != std::string::npos)
		{
			m_autoCirclesCache.push_back(circle);
		}
	}
	return m_autoCirclesCache;
}

std::vector<Circle> Debug::GetAllAutoFillCircles() const
{
	m_autoFillCirclesCache.clear();
	for (const Circle& circle : m_fillCirclesVector)
	{
		if (circle.GetId().find("fillcircle_auto_") != std::string::npos)
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

void Debug::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
	DrawPoint(f_render, f_window, p_camera);
	DrawLine(f_render, f_window, p_camera);
	DrawRect(f_render, f_window, p_camera);
	DrawFillRect(f_render, f_window, p_camera);
	DrawCircle(f_render, f_window, p_camera);
	DrawFillCircle(f_render, f_window, p_camera);
	DrawText(f_render, f_window, p_camera);
}

void Debug::DrawPoint(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& point : m_pointsVector)
	{
		if (point.GetIsClamp() == true)
		{
			Vector2f pointPos = point.GetPosition();
			if (pointPos.GetX() < 0)
			{
				point.SetPosition({ static_cast<float>(0 + m_margin), point.GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (pointPos.GetX() > windowW)
			{
				point.SetPosition({ static_cast<float>(windowW - m_margin), point.GetPosition().GetY() }, { 1, 0.5f });
			}

			if (pointPos.GetY() < 0)
			{
				point.SetPosition({ point.GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (pointPos.GetY() > windowH)
			{
				point.SetPosition({ point.GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		point.Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawLine(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& line : m_linesVector)
	{
		if (line.GetIsClamp() == true)
		{
			Vector2f lineStartPos = line.GetPosition();
			Vector2f lineEndPos = line.GetEndPosition();
			if (lineStartPos.GetX() < 0)
			{
				line.SetPosition({ static_cast<float>(0 + m_margin), line.GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (lineStartPos.GetX() > windowW)
			{
				line.SetPosition({ static_cast<float>(windowW - m_margin), line.GetPosition().GetY() }, { 1, 0.5f });
			}

			if (lineStartPos.GetY() < 0)
			{
				line.SetPosition({ line.GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (lineStartPos.GetY() > windowH)
			{
				line.SetPosition({ line.GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}

			if (lineEndPos.GetX() < 0)
			{
				line.SetEndPosition({ static_cast<float>(0 + m_margin), line.GetEndPosition().GetY() }, { 0, 0.5f });
			}
			else if (lineEndPos.GetX() > windowW)
			{
				line.SetEndPosition({ static_cast<float>(windowW - m_margin), line.GetEndPosition().GetY() }, { 1, 0.5f });
			}

			if (lineEndPos.GetY() < 0)
			{
				line.SetEndPosition({ line.GetEndPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (lineEndPos.GetY() > windowH)
			{
				line.SetEndPosition({ line.GetEndPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		line.Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawRect(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& rectangle : m_rectsVector)
	{
		if (rectangle.GetIsClamp() == true)
		{
			int width = rectangle.GetDimension().GetX();
			int height = rectangle.GetDimension().GetY();

			if (rectangle.GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				rectangle.SetPosition({ static_cast<float>(0 + m_margin), rectangle.GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (rectangle.GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				rectangle.SetPosition({ static_cast<float>(windowW - m_margin), rectangle.GetPosition().GetY() }, { 1, 0.5f });
			}
			if (rectangle.GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				rectangle.SetPosition({ rectangle.GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (rectangle.GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				rectangle.SetPosition({ rectangle.GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		rectangle.Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawFillRect(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& rectangle : m_fillRectsVector)
	{
		if (rectangle.GetIsClamp() == true)
		{
			int width = rectangle.GetDimension().GetX();
			int height = rectangle.GetDimension().GetY();

			if (rectangle.GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				rectangle.SetPosition({ static_cast<float>(0 + m_margin), rectangle.GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (rectangle.GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				rectangle.SetPosition({ static_cast<float>(windowW - m_margin), rectangle.GetPosition().GetY() }, { 1, 0.5f });
			}
			if (rectangle.GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				rectangle.SetPosition({ rectangle.GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (rectangle.GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				rectangle.SetPosition({ rectangle.GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		rectangle.Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawCircle(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& circle : m_circlesVector)
	{
		int width = circle.GetDimension().GetX();
		int height = circle.GetDimension().GetY();
		if (circle.GetIsClamp() == true)
		{
			if (circle.GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				circle.SetPosition({ static_cast<float>(0 + m_margin), circle.GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (circle.GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				circle.SetPosition({ static_cast<float>(windowW - m_margin), circle.GetPosition().GetY() }, { 1, 0.5f });
			}
			if (circle.GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				circle.SetPosition({ circle.GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (circle.GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				circle.SetPosition({ circle.GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		circle.Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawFillCircle(Render* f_render, Window* f_window, Camera* p_camera)
{
	int windowW = f_window->GetWindowSize().GetX();
	int windowH = f_window->GetWindowSize().GetY();
	for (auto& circle : m_fillCirclesVector)
	{
		int width = circle.GetDimension().GetX();
		int height = circle.GetDimension().GetY();
		if (circle.GetIsClamp() == true)
		{
			if (circle.GetPosition({ 0.0f, 0.5f }).GetX() < 0)
			{
				circle.SetPosition({ static_cast<float>(0 + m_margin), circle.GetPosition().GetY() }, { 0, 0.5f });
			}
			else if (circle.GetPosition({ 1.0f, 0.5f }).GetX() > windowW)
			{
				circle.SetPosition({ static_cast<float>(windowW - m_margin), circle.GetPosition().GetY() }, { 1, 0.5f });
			}
			if (circle.GetPosition({ 0.5f, 0.0f }).GetY() < 0)
			{
				circle.SetPosition({ circle.GetPosition().GetX(), static_cast<float>(0 + m_margin) }, { 0.5f, 0 });
			}
			else if (circle.GetPosition({ 0.5f, 1.0f }).GetY() > windowH)
			{
				circle.SetPosition({ circle.GetPosition().GetX(), static_cast<float>(windowH - m_margin) }, { 0.5f, 1 });
			}
		}
		circle.Draw(f_render, f_window, p_camera);
	}
}

void Debug::DrawText(Render* f_render, Window* f_window, Camera* p_camera)
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