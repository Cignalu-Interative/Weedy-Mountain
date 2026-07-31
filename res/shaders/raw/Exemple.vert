cbuffer RTData : register (b0, space1){
    float2 RenderTargetSize;
    float2 _Padding;
}

cbuffer UBO : register (b1, space1){
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

struct VS_INPUT {
    float2 Position : TEXCOORD0;  
    float4 Color    : TEXCOORD1;     
    float2 UV       : TEXCOORD2; 
};

struct PS_INPUT {
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    
    float2 ndc = (input.Position / RenderTargetSize) * 2.0 - 1.0;
    ndc.y = -ndc.y;

    output.Position = float4(ndc, 0.0, 1.0);
    output.UV = input.UV;
    
    return output;
}