cbuffer UBO : register (b0, space3){
    float Time;
    float Intensity;
    float2 Pos1;

    float2 Pos2;
    float2 _Padding0;
    
    float A;
    float B;
    int C;
    int D;
}

Texture2D MainTexture : register(t0, space2);
SamplerState MainSampler : register(s0, space2);

struct PS_INPUT {
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET {
    float2 uv = input.UV;

    float amplitude = (Intensity > 0.0) ? Intensity : 0.005;

    float stableTime = fmod(Time, 62.83185);

    float offsetX = sin(uv.y * 15.0 + stableTime * 3.0) * amplitude;
    float offsetY = cos(uv.x * 12.0 + stableTime * 2.5) * amplitude;

    float2 distortedUV = uv + float2(offsetX, offsetY);

    if (distortedUV.x < 0.0 || distortedUV.x > 1.0 || distortedUV.y < 0.0 || distortedUV.y > 1.0) {
        return float4(0.0, 0.0, 0.0, 0.0);
    }

    return MainTexture.Sample(MainSampler, distortedUV);
}