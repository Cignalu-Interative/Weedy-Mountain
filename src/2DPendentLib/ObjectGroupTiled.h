#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "tinyxml2.h"
#include "PropertyTiled.h"
#include "ObjectTiled.h"
#include "XMLSafe.h"

class MapTiled;
class GroupTiled;
class ObjectGroupTiled
{
private:

	std::string m_ObjectGroupColor;
	std::string m_ObjectGroupDrawOrder;
	std::string m_ObjectGroupName;
	std::string m_ObjectGroupClass;
	std::string m_ObjectGroupTintColor;

	std::vector<ObjectTiled*> m_objects;
	std::vector<PropertyTiled*> m_properties;

	Vector2f m_ObjectGroupOffset;
	Vector2f m_ObjectGroupParallaxFactor;

	int m_ObjectGroupId;

	float m_ObjectGroupOpacity;

	bool m_ObjectGroupVisible;
	bool m_ObjectGroupLocked;

	void ReadObjectGroup(tinyxml2::XMLElement* p_mapElement);

	void ReadObjectGroupProperties(tinyxml2::XMLElement* p_objectGroupElement);

	void ReadObject(tinyxml2::XMLElement* p_objectGroupElement, MapTiled* p_map);

public:

	friend class MapTiled;
	friend class GroupTiled;

	ObjectGroupTiled();
	~ObjectGroupTiled();

	PropertyTiled* GetObjectGroupProperty(const std::string& p_propertyName) const;

	std::string GetObjectGroupColor() const { return m_ObjectGroupColor; }
	std::string GetObjectGroupDrawOrder() const { return m_ObjectGroupDrawOrder; }
	int GetObjectGroupId() const { return m_ObjectGroupId; }
	std::string GetObjectGroupName() const { return m_ObjectGroupName; }
	std::string GetObjectGroupClass() const { return m_ObjectGroupClass; }
	bool IsObjectGroupVisible() const { return m_ObjectGroupVisible; }
	bool IsObjectGroupLocked() const { return m_ObjectGroupLocked; }
	float GetObjectGroupOpacity() const { return m_ObjectGroupOpacity; }
	std::string GetObjectGroupTintColor() const { return m_ObjectGroupTintColor; }
	Vector2f GetObjectGroupOffset() const { return m_ObjectGroupOffset; }
	Vector2f GetObjectGroupParallaxFactor() const { return m_ObjectGroupParallaxFactor; }

	std::vector<ObjectTiled*> GetAllObjects() { return m_objects; }
	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }
	std::vector<ObjectTiled*> GetObjectTiledByType(const std::string& p_type);

};

