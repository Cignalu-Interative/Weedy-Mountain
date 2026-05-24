#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "Vector2.h"
#include "tinyxml2.h"
#include "PropertyTiled.h"
#include "XMLSafe.h"
#include "MapTiled.h"

struct ObjectText
{
	std::string s_ObjectTextFontFamily;
	std::string s_ObjectTextColor;
	std::string s_ObjectTextHAlign;
	std::string s_ObjectTextVAlign;
	std::string s_ObjectTextContent;

	bool s_ObjectTextWrap;
};

struct ObjectPolygon
{
	std::vector<Vector2f> s_ObjectPolygonPoints;

	ObjectPolygon()
	{
		s_ObjectPolygonPoints.reserve(8); // Réserve de l'espace pour 8 points, ce qui correspond à un polygone avec 4 sommets (x, y) pairs
	}
};

class ObjectTiled
{
private:

	std::string m_ObjectName;
	std::string m_ObjectType;
	Vector2f m_ObjectPosition;
	Vector2f m_ObjectDimension;

	int m_ObjectRotation;
	int m_ObjectId;

	std::vector<PropertyTiled*> m_properties;

	ObjectText* m_ObjectText = nullptr;
	ObjectPolygon* m_ObjectPolygon = nullptr;

	bool m_ObjectVisible;
	bool m_ObjectEllipse = false;
	bool m_ObjectPoint = false;

	void ReadObject(tinyxml2::XMLElement* p_objectGroupElement, MapTiled* p_map);

	void ReadObjectProperties(tinyxml2::XMLElement* p_objectElement);

	void ReadObjectText(tinyxml2::XMLElement* p_objectElement);

	void ReadObjectPolygon(tinyxml2::XMLElement* p_objectElement);

public:

	friend class MapTiled;
	friend class ObjectGroupTiled;

	ObjectTiled();
	~ObjectTiled();

	PropertyTiled* GetObjectProperty(const std::string& p_propertyName) const;

	int GetObjectId() const { return m_ObjectId; }
	std::string GetObjectName() const { return m_ObjectName; }

	std::string GetObjectType() const { return m_ObjectType; }
	Vector2f GetObjectPosition() const { return m_ObjectPosition; }
	Vector2f GetObjectDimension() const { return m_ObjectDimension; }
	
	int GetObjectRotation() const { return m_ObjectRotation; }
	bool IsObjectVisible() const { return m_ObjectVisible; }
	ObjectText* GetObjectText() const { return m_ObjectText; }
	ObjectPolygon* GetObjectPolygon() const { return m_ObjectPolygon; }
	bool IsObjectEllipse() const { return m_ObjectEllipse; }
	bool IsObjectPoint() const { return m_ObjectPoint; }

	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }
};