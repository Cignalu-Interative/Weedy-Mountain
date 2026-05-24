#include "pch.h"
#include "Aleatory.h"
#include <2DPendentLib/Utils.h>

void Aleatory::Initialize()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int Aleatory::GetInt(int p_min, int p_max)
{
    if (p_min >= p_max)
    {
        std::cerr << "Engine/Aleatory.h/GetInt/p_min >= p_max" << std::endl;
        return 0;
    }
    return p_min + std::rand() % (p_max - p_min + 1);
}

float Aleatory::GetFloat(float p_min, float p_max)
{
    if (p_min >= p_max)
    {
        std::cerr << "Engine/Aleatory.h/GetFloat/p_min >= p_max" << std::endl;
        return 0.0f;
    }
    return p_min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (p_max - p_min));
}

unsigned int Aleatory::GetUInt(unsigned int p_min, unsigned int p_max)
{
    if (p_min >= p_max)
    {
        std::cerr << "Engine/Aleatory.h/GetUInt/p_min >= p_max" << std::endl;
        return 0;
    }
    if (p_min < 0 || p_max < 0)
    {
        std::cerr << "Engine/Aleatory.h/GetUInt/p_min < 0 || p_max < 0" << std::endl;
        return 0;
    }
    return static_cast<unsigned int>(p_min + rand() % (p_max - p_min + 1));
}

char Aleatory::GetCapitalChar()
{
    return static_cast<char>(65 + std::rand() % (90 - 65 + 1));
}

char Aleatory::GetTinyChar()
{
    return static_cast<char>(97 + std::rand() % (122 - 97 + 1));
}

Vector2f Aleatory::GetVector2f(Vector2f p_minMaxX, Vector2f p_minMaxY)
{
    float x = GetFloat(p_minMaxX.GetX(), p_minMaxX.GetY());
    float y = GetFloat(p_minMaxY.GetX(), p_minMaxY.GetY());
    return Vector2f(x, y);
}

Vector2i Aleatory::GetVector2i(Vector2i p_minMaxX, Vector2i p_minMaxY)
{
    int x = GetInt(p_minMaxX.GetX(), p_minMaxX.GetY());
    int y = GetInt(p_minMaxY.GetX(), p_minMaxY.GetY());
    return Vector2i(x, y);
}

Vector2u Aleatory::GetVector2u(Vector2u p_minMaxX, Vector2u p_minMaxY)
{
    unsigned int x = GetUInt(p_minMaxX.GetX(), p_minMaxX.GetY());
    unsigned int y = GetUInt(p_minMaxY.GetX(), p_minMaxY.GetY());
    return Vector2u(x, y);
}

bool Aleatory::GetBool(float p_trueProbability)
{
    return GetFloat(0.0f, 1.0f) < p_trueProbability;
}