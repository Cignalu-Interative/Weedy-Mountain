cbuffer UniformBlock : register(b0, space1)
{
    float2 rtSize;
    float2 padding; 
};

struct VertexInput
{
    float2 pos      : POSITION;  
    float4 color    : COLOR;     
    float2 texCoord : TEXCOORD;  
};

struct VertexOutput
{
    float4 color    : COLOR0;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_Position; 
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;

    float ncx = (input.pos.x / rtSize.x) * 2.0f - 1.0f;
    float ncy = 1.0f - (input.pos.y / rtSize.y) * 2.0f; 

    output.position = float4(ncx, ncy, 0.0f, 1.0f);
    output.color = input.color;
    output.texCoord = input.texCoord;

    return output;
}