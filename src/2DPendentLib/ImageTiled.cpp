#include "pch.h"
#include "ImageTiled.h"
#include "ImageLayerTiled.h"
#include "MapTiled.h"
#include "GroupTiled.h"

void ImageTiled::ReadImage(tinyxml2::XMLElement* p_imageElement)
{
	m_ImageSource = XMLSafe::GetString(p_imageElement, "source");
	m_ImageColorTransparent = XMLSafe::GetString(p_imageElement, "trans");
	m_ImageDimension = { XMLSafe::GetInt(p_imageElement, "width"), XMLSafe::GetInt(p_imageElement, "height") };
}