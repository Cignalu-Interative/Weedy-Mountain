#include "pch.h"
#include "ObjectGroupTiled.h"
#include "MapTiled.h"
#include "GroupTiled.h"

void ObjectGroupTiled::ReadObjectGroup(tinyxml2::XMLElement* p_mapElement)
{
	m_ObjectGroupColor = XMLSafe::GetString(p_mapElement, "color");
	m_ObjectGroupDrawOrder = XMLSafe::GetString(p_mapElement, "draworder");
	m_ObjectGroupId = XMLSafe::GetInt(p_mapElement, "id");
	m_ObjectGroupName = XMLSafe::GetString(p_mapElement, "name");
	m_ObjectGroupClass = XMLSafe::GetString(p_mapElement, "class");
	m_ObjectGroupVisible = XMLSafe::GetBool(p_mapElement, "visible", true);
	m_ObjectGroupLocked = XMLSafe::GetBool(p_mapElement, "locked", false);
	m_ObjectGroupOpacity = XMLSafe::GetFloat(p_mapElement, "opacity", 1.0f);
	m_ObjectGroupTintColor = XMLSafe::GetString(p_mapElement, "tintcolor");
	m_ObjectGroupOffset = { XMLSafe::GetFloat(p_mapElement, "offsetx", 0.0f), XMLSafe::GetFloat(p_mapElement, "offsety", 0.0f) };
	m_ObjectGroupParallaxFactor = { XMLSafe::GetFloat(p_mapElement, "parallaxx", 1.0f), XMLSafe::GetFloat(p_mapElement, "parallaxy", 1.0f) };
}

void ObjectGroupTiled::ReadObjectGroupProperties(tinyxml2::XMLElement* p_objectGroupElement)
{
    tinyxml2::XMLElement* propertiesElement = p_objectGroupElement->FirstChildElement();
    while (propertiesElement)
    {
        std::string tagNameProperties = propertiesElement->Value();
        if (tagNameProperties == "property")
        {
            PropertyTiled* tempProperty = new PropertyTiled();
            tempProperty->m_PropertyName = XMLSafe::GetString(propertiesElement, "name");
            tempProperty->m_PropertyType = XMLSafe::GetString(propertiesElement, "type");
            std::string valueStr = XMLSafe::GetString(propertiesElement, "value");

            tempProperty->ReadPropertyValue(valueStr);

            m_properties.push_back(tempProperty);
        }
        propertiesElement = propertiesElement->NextSiblingElement();
    }
}

void ObjectGroupTiled::ReadObject(tinyxml2::XMLElement* p_objectGroupElement, MapTiled* p_map)
{
    ObjectTiled* tempObject = new ObjectTiled();
    tempObject->ReadObject(p_objectGroupElement, p_map);
    m_objects.push_back(tempObject);
}

ObjectGroupTiled::ObjectGroupTiled()
{
    m_objects.reserve(48);    // objets par groupe (spawns, triggers, colliders...)
    m_properties.reserve(8);  // propriétés custom du groupe
}

ObjectGroupTiled::~ObjectGroupTiled()
{
    for (ObjectTiled* object : m_objects)
    {
        delete object;
    }
    m_objects.clear();
    for (PropertyTiled* property : m_properties)
    {
        delete property;
    }
	m_properties.clear();
}

PropertyTiled* ObjectGroupTiled::GetObjectGroupProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}

std::vector<ObjectTiled*> ObjectGroupTiled::GetObjectTiledByType(const std::string& p_type)
{
    std::vector<ObjectTiled*> objectG;
    for (ObjectTiled* objectGroupe : m_objects)
    {
        if (objectGroupe->m_ObjectType == p_type)
        {
            objectG.push_back(objectGroupe);
        }
    }
    return objectG;
}
