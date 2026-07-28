struct Input
{
    float4 color : TEXCOORD0;
    float2 UV : TEXCOORD1;
    float4 Position : SV_Position;
};


Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

float4 main(Input input) : SV_Target0
{
    float4 texColor = Texture.Sample(Sampler, input.UV);
    return texColor * input.color;
}   