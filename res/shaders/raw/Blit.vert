struct Input
{
    float2 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct Output
{
    float4 Position : SV_Position;
    float4 color : TEXCOORD0;
    float2 UV : TEXCOORD1;
};

Output main(Input input)
{
    Output output;
    output.Position = float4(input.Position, 0.0f, 1.0f); 
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.UV = input.UV;
    return output;
}