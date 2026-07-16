// Structure d'entrée depuis le vertex shader
struct PSInput
{
    float4 position : SV_Position;
};

// Couleur de sortie (rouge pour tester)
float4 main(PSInput input) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f); // Rouge opaque
}