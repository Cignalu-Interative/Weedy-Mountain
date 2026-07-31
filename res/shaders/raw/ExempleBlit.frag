cbuffer UBO : register (b0, space3){
    float Time;
}

Texture2D    RenderTargetTex : register(t0, space2);
SamplerState RenderTargetSampler : register(s0, space2);

struct Input {
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_Target0 {

    float VignetteStrength = 0.6; // ex: 0.6
    float TintAmount = 0.3;       // ex: 0.3 (0 = niveaux de gris purs, 1 = couleur d'origine)
    float2 _padding = {0, 0};

    float4 color = RenderTargetTex.Sample(RenderTargetSampler, input.uv);

    // Niveaux de gris (luminance)
    float gray = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
    float3 grayColor = float3(gray, gray, gray);

    // Mélange gris / couleur d'origine, avec une légère teinte bleue pour bien voir l'effet
    float3 tinted = lerp(grayColor, color.rgb, TintAmount) * float3(0.9f, 0.95f, 1.15f);

    // Vignette simple basée sur la distance au centre de l'écran
    float2 centered = input.uv - 0.5f;
    float dist = length(centered);
    float vignette = smoothstep(0.75f, 0.2f, dist * (1.0f + VignetteStrength));

    float3 finalColor = tinted * vignette;

    return float4(finalColor, color.a);
}