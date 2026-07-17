struct VS_Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VS_Output main(uint vertexID : SV_VertexID)
{
    VS_Output output;

    // Astuce : 3 sommets qui couvrent tout l'écran (NDC)
    float2 positions[3] =
    {
        float2(-1.0, 1.0), // Haut gauche
        float2(3.0, 1.0), // Haut droite (déborde de l'écran)
        float2(-1.0, -3.0) // Bas gauche (déborde de l'écran)
    };

    // Coordonnées de texture correspondantes
    float2 uvs[3] =
    {
        float2(0.0, 0.0),
        float2(2.0, 0.0),
        float2(0.0, 2.0)
    };

    output.pos = float4(positions[vertexID], 0.0, 1.0);
    output.uv = uvs[vertexID];
    return output;
}