#pragma once

template<typename T>
class Condition
{
public:

	int ExpectedValueInt = 0; // Valeur entière attendue pour la condition
	float ExpectedValueFloat = 0.0f; // Valeur flottante attendue pour la condition
	bool ExpectedValue = true; // Valeur booléenne attendue pour la condition

	// Fonction Pour tester la condition en fonction du type de T
    bool InternalTest(T* type)
    {
        if constexpr (std::is_same_v<T, int>)
        {
            return TestInt(type) >= ExpectedValueInt;
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            return TestFloat(type) >= ExpectedValueFloat;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return TestBool(type) == ExpectedValue;
		}
    }

	// Fonctions de test virtuelles en Int
	virtual int TestInt(T* type) { return 0; }
	// Fonctions de test virtuelles en Float
	virtual float TestFloat(T* type) { return 0.0f; }
	// Fonctions de test virtuelles en Bool
	virtual bool TestBool(T* type) = 0;
};