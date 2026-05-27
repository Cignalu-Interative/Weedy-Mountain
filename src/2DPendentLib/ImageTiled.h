#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "tinyxml2.h"
#include "XMLSafe.h"

class ImageLayerTiled;
class MapTiled;
class GroupTiled;
class ImageTiled
{
private:

	std::string m_ImageSource;
	std::string m_ImageColorTransparent;
	Vector2i m_ImageDimension;

	void ReadImage(tinyxml2::XMLElement* p_imageElement);

public:

	friend class ImageLayerTiled;
	friend class MapTiled;
	friend class GroupTiled;

	ImageTiled() = default;
	~ImageTiled() = default;

	std::string GetImageSource() const { return m_ImageSource; }
	std::string GetImageColorTransparent() const { return m_ImageColorTransparent; }
	Vector2i GetImageDimension() const { return m_ImageDimension; }
};