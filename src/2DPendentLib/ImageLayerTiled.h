#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "tinyxml2.h"
#include "PropertyTiled.h"
#include "XMLSafe.h"
#include "ImageTiled.h"

class MapTiled;
class GroupTiled;
class ImageLayerTiled
{
private:

	std::string m_ImageLayerName;
	std::string m_ImageLayerClass;
	std::string m_ImageLayerTintColor;

	std::vector<PropertyTiled*> m_properties;

	ImageTiled* m_ImageLayerImage = nullptr;

	Vector2f m_ImageLayerOffset;
	Vector2f m_ImageLayerParallaxFactor;

	int m_ImageLayerId;

	float m_ImageLayerOpacity;

	bool m_ImageLayerVisible;
	bool m_ImageLayerLocked;
	bool m_ImageLayerRepeatX;
	bool m_ImageLayerRepeatY;

	void ReadImageLayer(tinyxml2::XMLElement* p_mapElement);

	void ReadImageLayerProperties(tinyxml2::XMLElement* p_imageLayerElement);

public:

	friend class MapTiled;
	friend class GroupTiled;

	ImageLayerTiled();
	~ImageLayerTiled();

	PropertyTiled* GetImageLayerProperty(const std::string& p_propertyName) const;

	int GetImageLayerId() const { return m_ImageLayerId; }
	std::string GetImageLayerName() const { return m_ImageLayerName; }
	std::string GetImageLayerClass() const { return m_ImageLayerClass; }
	bool IsImageLayerVisible() const { return m_ImageLayerVisible; }
	bool IsImageLayerLocked() const { return m_ImageLayerLocked; }
	float GetImageLayerOpacity() const { return m_ImageLayerOpacity; }
	std::string GetImageLayerTintColor() const { return m_ImageLayerTintColor; }
	Vector2f GetImageLayerOffset() const { return m_ImageLayerOffset; }
	Vector2f GetImageLayerParallaxFactor() const { return m_ImageLayerParallaxFactor; }
	bool IsImageLayerRepeatX() const { return m_ImageLayerRepeatX; }
	bool IsImageLayerRepeatY() const { return m_ImageLayerRepeatY; }
	ImageTiled* GetImageTiled() const { return m_ImageLayerImage; }

	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }


};

