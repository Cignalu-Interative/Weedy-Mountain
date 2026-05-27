#pragma once
#include "tinyxml2.h"
#include <string>

class XMLSafe {
public:

    //STRING safe
    // Return la valeur de l'attribut p_name de l'élément p_element, ou p_default si l'élément est null ou si l'attribut n'existe pas.
    static std::string GetString(tinyxml2::XMLElement* p_element, const char* p_name, const std::string& p_default = "unknown")
    {
        if (!p_element)
        {
            return p_default;
        }
        const char* val = p_element->Attribute(p_name);
        return val ? std::string(val) : p_default;
    }

    //INT safe
    // Return la valeur de l'attribut p_name de l'élément p_element, ou p_default si l'élément est null ou si l'attribut n'existe pas ou si la conversion échoue.
    static int GetInt(tinyxml2::XMLElement* p_element, const char* p_name, int p_default = 0)
    {
        if (!p_element)
        {
            return p_default;
        }
        int val = p_default;
        p_element->QueryIntAttribute(p_name, &val);
        return val;
    }

    //FLOAT safe
    // Return la valeur de l'attribut p_name de l'élément p_element, ou p_default si l'élément est null ou si l'attribut n'existe pas ou si la conversion échoue.
    static float GetFloat(tinyxml2::XMLElement* p_element, const char* p_name, float p_default = 0.0f)
    {
        if (!p_element)
        {
            return p_default;
        }
        float val = p_default;
        p_element->QueryFloatAttribute(p_name, &val);
        return val;
    }

    // BOOL safe
    // Return la valeur de l'attribut p_name de l'élément p_element, ou p_default si l'élément est null ou si l'attribut n'existe pas ou si la conversion échoue.
    static bool GetBool(tinyxml2::XMLElement* p_element, const char* p_name, bool p_default = false)
    {
        if (!p_element)
        {
            return p_default;
        }
        return p_element->BoolAttribute(p_name, p_default);
    }
};