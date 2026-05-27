#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "tinyxml2.h"
#include "PropertyTiled.h"
#include "ObjectTiled.h"
#include "XMLSafe.h"
#include "LayerTiled.h"

class MapTiled;
class GroupTiled
{
private:

	std::string m_GroupId;
	std::string m_GroupName;
	std::string m_GroupClass;
	std::string m_GroupTintColor;

	std::vector<PropertyTiled*> m_properties;
	std::vector<GroupTiled*> m_groups;
	std::vector<LayerTiled*> m_layers;
	std::vector<ObjectGroupTiled*> m_objectGroups;
	std::vector<ImageLayerTiled*> m_imageLayers;

	Vector2f m_GroupOffset;
	Vector2f m_GroupParallaxFactor;

	float m_GroupOpacity;

	bool m_GroupVisible;
	bool m_GroupLocked;

	void ReadGroup(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map);

	void ReadLayer(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map);
	void ReadObjectGroup(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map);
	void ReadImageLayer(tinyxml2::XMLElement* p_mapElement);
	void ReadGroupElement(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map);

	void ReadGroupProperties(tinyxml2::XMLElement* p_groupElement);

public:

	friend class MapTiled;

	GroupTiled();
	~GroupTiled();

	PropertyTiled* GetGroupProperty(const std::string& p_propertyName) const;

	std::string GetGroupId() const { return m_GroupId; }
	std::string GetGroupName() const { return m_GroupName; }
	std::string GetGroupClass() const { return m_GroupClass; }
	bool IsGroupVisible() const { return m_GroupVisible; }
	bool IsGroupLocked() const { return m_GroupLocked; }
	float GetGroupOpacity() const { return m_GroupOpacity; }
	std::string GetGroupTintColor() const { return m_GroupTintColor; }
	Vector2f GetGroupOffset() const { return m_GroupOffset; }
	Vector2f GetGroupParallaxFactor() const { return m_GroupParallaxFactor; }

	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }
	std::vector<GroupTiled*> GetAllGroups() { return m_groups; }
	std::vector<LayerTiled*> GetAllLayers() { return m_layers; }
	std::vector<ObjectGroupTiled*> GetAllObjectGroups() { return m_objectGroups; }
	std::vector<ImageLayerTiled*> GetAllImageLayers() { return m_imageLayers; }

	GroupTiled* GetGroupByName(const std::string& p_name);
	LayerTiled* GetLayerByName(const std::string& p_name);
	ObjectGroupTiled* GetObjectGroupByName(const std::string& p_name);
	ImageLayerTiled* GetImageLayerByName(const std::string& p_name);
};