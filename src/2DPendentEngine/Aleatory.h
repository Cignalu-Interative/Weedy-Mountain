#pragma once
#include <cstdlib>
#include <ctime>
#include "2DPendentLib/Vector2.h"

class Aleatory
{
public:

    //Destructeur de Aleatory
    ~Aleatory() = default;

    //Initialiser le générateur de nombres aléatoires avec une graine basée sur l'heure actuelle
    static void Initialize();

    // Générer un nombre entier aléatoire entre min et max (inclus)
    static int GetInt(int p_min, int p_max);

    // Générer un nombre flottant aléatoire entre min et max (inclus)
    static float GetFloat(float p_min, float p_max);

    // Générer un nombre entier non signé aléatoire entre min et max (inclus)
    static unsigned int GetUInt(unsigned int p_min, unsigned int p_max);

    // Générer un caractère majuscule aléatoire (A-Z)
    static char GetCapitalChar();

    // Générer un caractère minuscule aléatoire (a-z)
    static char GetTinyChar();

    // Générer Vector2f aléatoire avec des composantes x et y entre les valeurs min et max spécifiées dans les Vector2f p_minMaxX et p_minMaxY
    static Vector2f GetVector2f(Vector2f p_minMaxX, Vector2f p_minMaxY);

    // Générer Vector2i aléatoire avec des composantes x et y entre les valeurs min et max spécifiées dans les Vector2i p_minMaxX et p_minMaxY
    static Vector2i GetVector2i(Vector2i p_minMaxX, Vector2i p_minMaxY);

    // Générer Vector2u aléatoire avec des composantes x et y entre les valeurs min et max spécifiées dans les Vector2u p_minMaxX et p_minMaxY
    static Vector2u GetVector2u(Vector2u p_minMaxX, Vector2u p_minMaxY);

    // Générer un booléen aléatoire avec une probabilité spécifiée pour true
	// Return true si le nombre aléatoire généré est inférieur à la probabilité de true, sinon retourne false
    static bool GetBool(float p_trueProbability = 0.5f);
};