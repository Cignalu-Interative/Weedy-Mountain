#include "pch.h"
#include "ImageLayerTiled.h"
#include "MapTiled.h"
#include "GroupTiled.h"

void ImageLayerTiled::ReadImageLayer(tinyxml2::XMLElement* p_mapElement)
{
	m_ImageLayerId = XMLSafe::GetInt(p_mapElement, "id");
	m_ImageLayerName = XMLSafe::GetString(p_mapElement, "name");
	m_ImageLayerClass = XMLSafe::GetString(p_mapElement, "class");
	m_ImageLayerVisible = XMLSafe::GetBool(p_mapElement, "visible", true);
	m_ImageLayerLocked = XMLSafe::GetBool(p_mapElement, "locked", false);
	m_ImageLayerOpacity = XMLSafe::GetFloat(p_mapElement, "opacity", 1.0f);
	m_ImageLayerTintColor = XMLSafe::GetString(p_mapElement, "tintcolor");
	m_ImageLayerOffset = { XMLSafe::GetFloat(p_mapElement, "offsetx", 0.0f), XMLSafe::GetFloat(p_mapElement, "offsety", 0.0f) };
	m_ImageLayerParallaxFactor = { XMLSafe::GetFloat(p_mapElement, "parallaxx", 1.0f), XMLSafe::GetFloat(p_mapElement, "parallaxy", 1.0f) };
	m_ImageLayerRepeatX = XMLSafe::GetBool(p_mapElement, "repeatx", false);
	m_ImageLayerRepeatY = XMLSafe::GetBool(p_mapElement, "repeaty", false);
}

void ImageLayerTiled::ReadImageLayerProperties(tinyxml2::XMLElement* p_imageLayerElement)
{
    tinyxml2::XMLElement* propertiesElement = p_imageLayerElement->FirstChildElement();
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

ImageLayerTiled::ImageLayerTiled()
{
    m_properties.reserve(8); // quelques propriétés custom par image layer
}

ImageLayerTiled::~ImageLayerTiled()
{
    for (PropertyTiled* prop : m_properties)
    {
        delete prop;
    }
	m_properties.clear();
}

PropertyTiled* ImageLayerTiled::GetImageLayerProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}
