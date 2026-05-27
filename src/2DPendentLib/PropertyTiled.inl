#pragma once
#include <iostream>

template<typename T>
T PropertyTiled::GetPropertyValue() const
{
    if (HasValue() == false)
    {
        std::cout << "Propriété <" << m_PropertyName << "> non trouvée ou type non défini." << std::endl;
        return T{}; // Valeur par défaut si la propriété n'est pas trouvée ou si le type n'est pas défini
	}
    if (auto* value = std::get_if<T>(&m_PropertyValue))
    {
        return *value;
    }
    return T{};
}