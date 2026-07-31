cbuffer UBO : register (b0, space3){
    float Time;
}

struct Output {
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

Output main(uint vertexID : SV_VertexID) {
    Output output;

    float2 pos = float2((vertexID << 1) & 2, vertexID & 2);
    output.position = float4(pos * 2.0f - 1.0f, 0.0f, 1.0f);

    output.uv = float2(pos.x, 1.0f - pos.y);

    return output;
}