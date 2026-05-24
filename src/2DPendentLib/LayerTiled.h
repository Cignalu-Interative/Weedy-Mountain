#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "tinyxml2.h"
#include "PropertyTiled.h"
#include "XMLSafe.h"

class MapTiled;
class GroupTiled;
class LayerTiled
{
private:

	std::string m_LayerName;
	std::string m_LayerClass;
	std::string m_LayerTintColor;

	std::vector<int> m_LayerTiles; // la partie DATA le csv
	std::vector<PropertyTiled*> m_properties;

	Vector2i m_LayerDimension;
	Vector2f m_LayerOffset;
	Vector2f m_LayerParallaxFactor;

	int m_LayerId;

	float m_LayerOpacity;

	bool m_LayerVisible;
	bool m_LayerLocked;

	void ReadLayer(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map);

	void ReadData(tinyxml2::XMLElement* p_layerElement);

	void ReadLayerProperties(tinyxml2::XMLElement* p_layerElement);

public:

	friend class MapTiled;
	friend class GroupTiled;

	LayerTiled();
	~LayerTiled();

	PropertyTiled* GetLayerProperty(const std::string& p_propertyName) const;

	int GetLayerId() const { return m_LayerId; }
	std::string GetLayerName() const { return m_LayerName; }
	std::string GetLayerClass() const { return m_LayerClass; }
	Vector2i GetLayerDimension() const { return m_LayerDimension; }
	bool IsLayerVisible() const { return m_LayerVisible; }
	bool IsLayerLocked() const { return m_LayerLocked; }
	float GetLayerOpacity() const { return m_LayerOpacity; }
	std::string GetLayerTintColor() const { return m_LayerTintColor; }
	Vector2f GetLayerOffset() const { return m_LayerOffset; }
	Vector2f GetLayerParallaxFactor() const { return m_LayerParallaxFactor; }
	std::vector<int> GetLayerTiles() const { return m_LayerTiles; }

	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }
};