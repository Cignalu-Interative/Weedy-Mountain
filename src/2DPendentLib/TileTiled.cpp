#include "pch.h"
#include "TileTiled.h"

void TileTiled::ReadTile(tinyxml2::XMLElement* p_tileElement)
{
    m_TileId = XMLSafe::GetInt(p_tileElement, "id");
    m_TileType = XMLSafe::GetString(p_tileElement, "type");
    m_TileProbability = XMLSafe::GetInt(p_tileElement, "probability");
}

void TileTiled::ReadTileProperties(tinyxml2::XMLElement* p_tileElement)
{
    tinyxml2::XMLElement* propertiesElement = p_tileElement->FirstChildElement();
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

TileTiled::TileTiled()
{
    m_properties.reserve(8); // propriétés custom de la tile
}

TileTiled::~TileTiled()
{
    for (PropertyTiled* property : m_properties)
    {
        delete property;
    }
	m_properties.clear();
}

PropertyTiled* TileTiled::GetTileProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}