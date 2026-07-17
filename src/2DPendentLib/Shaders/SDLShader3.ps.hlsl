// On déclare notre texture et l'outil pour la lire (Sampler)
Texture2D uiTexture : register(t0);
SamplerState uiSampler : register(s0);

struct VS_Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(VS_Output input) : SV_Target
{
    // On lit simplement le pixel de la texture générée par ton SDL_Renderer
    return uiTexture.Sample(uiSampler, input.uv);
}