struct VS_Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(VS_Output input) : SV_Target
{
    return input.color; // On applique la couleur interpolée
}