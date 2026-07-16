// Structure d'entrée : position en 2D (x, y)
struct VSInput
{
    float2 position : POSITION;
};

// Structure de sortie : position en espace clip (x, y, z, w)
struct VSOutput
{
    float4 position : SV_Position;
};

// Matrice de projection (passée depuis le CPU)
cbuffer Constants : register(b0)
{
    float4x4 worldViewProj;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    // Convertir la position 2D en 4D (z=0, w=1) et appliquer la matrice
    output.position = mul(float4(input.position, 0.0f, 1.0f), worldViewProj);
    return output;
}