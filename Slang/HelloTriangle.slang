static float2 sPositions[3] = float2[](
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5));

static float3 sColors[3] = float3[](
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
);

struct VertexOutput
{
    float3 color;

    // SV_Position 标识 Vertex Shader 输出的裁剪空间位置
    // 后续固定功能阶段负责裁剪、透视除法和视口变换
    float4 position : SV_Position;
};

[shader("vertex")]
VertexOutput vertMain(uint vid: SV_VertexID)
{
    VertexOutput output;

    output.position = float4(
        sPositions[vid],
        0.0,
        1.0);
    output.color = sColors[vid];

    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target
{
    // 片段着色器直接输出顶点着色器传入的颜色
    return float4(input.color, 1.0);
}
