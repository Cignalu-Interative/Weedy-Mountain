struct VS_Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_Output main(uint vertexID : SV_VertexID)
{
    VS_Output output;

    // Les 3 sommets de notre triangle (coordonnées d'écran entre -1.0 et 1.0)
    float2 positions[3] =
    {
        float2(0.0, 0.5), // Sommet du haut
        float2(0.5, -0.5), // En bas à droite
        float2(-0.5, -0.5) // En bas à gauche
    };

    // Une couleur différente pour chaque angle (Rouge, Vert, Bleu)
    float4 colors[3] =
    {
        float4(1.0, 0.0, 0.0, 1.0), // Rouge
        float4(0.0, 1.0, 0.0, 1.0), // Vert
        float4(0.0, 0.0, 1.0, 1.0) // Bleu
    };

    output.pos = float4(positions[vertexID], 0.0, 1.0);
    output.color = colors[vertexID];
    return output;
}