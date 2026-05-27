#include "pch.h"
#include "ObjectTiled.h"

void ObjectTiled::ReadObject(tinyxml2::XMLElement* p_objectGroupElement, MapTiled* p_map)
{
    m_ObjectId = p_map->m_MapNextObjectId++;
    m_ObjectId = XMLSafe::GetInt(p_objectGroupElement, "id");
    m_ObjectName = XMLSafe::GetString(p_objectGroupElement, "name");
    m_ObjectType = XMLSafe::GetString(p_objectGroupElement, "type");
    m_ObjectPosition = { XMLSafe::GetFloat(p_objectGroupElement, "x"), XMLSafe::GetFloat(p_objectGroupElement, "y") };
    m_ObjectDimension = { XMLSafe::GetFloat(p_objectGroupElement, "width"), XMLSafe::GetFloat(p_objectGroupElement, "height") };
    m_ObjectRotation = XMLSafe::GetInt(p_objectGroupElement, "rotation");
    m_ObjectVisible = XMLSafe::GetBool(p_objectGroupElement, "visible", true);

    tinyxml2::XMLElement* objectElement = p_objectGroupElement->FirstChildElement();
    while (objectElement)
    {
        std::string tagNameObject = objectElement->Value();
        if (tagNameObject == "properties")
        {
            ReadObjectProperties(objectElement);
        }
        else if (tagNameObject == "text")
        {
            ReadObjectText(objectElement);
        }
        else if (tagNameObject == "polygon")
        {
            ReadObjectPolygon(objectElement);
        }
        else if (tagNameObject == "ellipse")
        {
            m_ObjectEllipse = true;
        }
        else if (tagNameObject == "point")
        {
            m_ObjectPoint = true;
		}
        objectElement = objectElement->NextSiblingElement();
    }
}

void ObjectTiled::ReadObjectProperties(tinyxml2::XMLElement* p_objectElement)
{
    tinyxml2::XMLElement* propertiesElement = p_objectElement->FirstChildElement();
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

void ObjectTiled::ReadObjectText(tinyxml2::XMLElement* p_objectElement)
{
    m_ObjectText = new ObjectText();
    tinyxml2::XMLElement* textElement = p_objectElement->FirstChildElement("text");
    if (textElement)
    {
        m_ObjectText->s_ObjectTextFontFamily = XMLSafe::GetString(textElement, "fontfamily");
        m_ObjectText->s_ObjectTextWrap = XMLSafe::GetBool(textElement, "wrap", false);
        m_ObjectText->s_ObjectTextColor = XMLSafe::GetString(textElement, "color");
        m_ObjectText->s_ObjectTextHAlign = XMLSafe::GetString(textElement, "halign");
        m_ObjectText->s_ObjectTextVAlign = XMLSafe::GetString(textElement, "valign");
        m_ObjectText->s_ObjectTextContent = textElement->GetText() ? textElement->GetText() : "";
	}
}

void ObjectTiled::ReadObjectPolygon(tinyxml2::XMLElement* p_objectElement)
{
    m_ObjectPolygon = new ObjectPolygon();
    std::string pointsStr = XMLSafe::GetString(p_objectElement, "points");
    std::stringstream ss(pointsStr);
    std::string point;
    while (std::getline(ss, point, ' '))
    {
        std::stringstream pointStream(point);
        std::string xStr, yStr;
        if (std::getline(pointStream, xStr, ',') && std::getline(pointStream, yStr))
        {
            float x = std::stof(xStr);
            float y = std::stof(yStr);
            m_ObjectPolygon->s_ObjectPolygonPoints.push_back({ x, y });
        }
    }
}

ObjectTiled::ObjectTiled()
{
    m_properties.reserve(8); // propriétés custom de l'objet
}

ObjectTiled::~ObjectTiled()
{
    for (PropertyTiled* property : m_properties)
    {
        delete property;
    }
    m_properties.clear();
    delete m_ObjectText;
    m_ObjectText = nullptr;
    delete m_ObjectPolygon;
	m_ObjectPolygon = nullptr;
}

PropertyTiled* ObjectTiled::GetObjectProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
    
}